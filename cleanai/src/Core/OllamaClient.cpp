#include "OllamaClient.h"

#include <algorithm>
#include <chrono>
#include <thread>
#include <vector>
#include <windows.h>

namespace
{
    using namespace std::chrono_literals;

    bool IsServerAvailable(web::http::client::http_client& client)
    {
        try
        {
            auto response = client.request(web::http::methods::GET, U("/api/tags")).get();
            return response.status_code() < 400;
        }
        catch (...)
        {
            return false;
        }
    }

    bool IsOllamaInstalled()
    {
        wchar_t buffer[MAX_PATH]{};
        return SearchPathW(nullptr, L"ollama.exe", nullptr, MAX_PATH, buffer, nullptr) > 0;
    }

    bool RunProcessAndWait(std::wstring const& commandLine)
    {
        STARTUPINFOW startupInfo{};
        startupInfo.cb = sizeof(startupInfo);
        PROCESS_INFORMATION processInfo{};

        auto command = commandLine;
        auto created = CreateProcessW(
            nullptr,
            command.data(),
            nullptr,
            nullptr,
            FALSE,
            CREATE_NO_WINDOW,
            nullptr,
            nullptr,
            &startupInfo,
            &processInfo);

        if (!created)
        {
            return false;
        }

        WaitForSingleObject(processInfo.hProcess, INFINITE);

        DWORD code{};
        GetExitCodeProcess(processInfo.hProcess, &code);
        CloseHandle(processInfo.hThread);
        CloseHandle(processInfo.hProcess);
        return code == 0;
    }

    bool RunProcessDetached(std::wstring const& commandLine)
    {
        STARTUPINFOW startupInfo{};
        startupInfo.cb = sizeof(startupInfo);
        PROCESS_INFORMATION processInfo{};

        auto command = commandLine;
        auto created = CreateProcessW(
            nullptr,
            command.data(),
            nullptr,
            nullptr,
            FALSE,
            DETACHED_PROCESS | CREATE_NEW_PROCESS_GROUP,
            nullptr,
            nullptr,
            &startupInfo,
            &processInfo);

        if (!created)
        {
            return false;
        }

        CloseHandle(processInfo.hThread);
        CloseHandle(processInfo.hProcess);
        return true;
    }

    bool WaitUntilServerReady(web::http::client::http_client& client, std::chrono::seconds timeout)
    {
        auto deadline = std::chrono::steady_clock::now() + timeout;
        while (std::chrono::steady_clock::now() < deadline)
        {
            if (IsServerAvailable(client))
            {
                return true;
            }

            std::this_thread::sleep_for(1s);
        }

        return false;
    }

    bool TryInstallOllama()
    {
        return RunProcessAndWait(
            L"winget install --id Ollama.Ollama -e --accept-package-agreements --accept-source-agreements");
    }

    void EnsureModelReady(web::http::client::http_client& client)
    {
        try
        {
            auto response = client.request(web::http::methods::GET, U("/api/tags")).get();
            if (response.status_code() >= 400)
            {
                return;
            }

            auto json = response.extract_json().get();
            if (!json.has_field(U("models")) || !json[U("models")].is_array())
            {
                return;
            }

            auto models = json[U("models")].as_array();
            auto hasTinyLlama = std::any_of(models.begin(), models.end(), [](web::json::value const& model)
            {
                if (!model.is_object() || !model.has_field(U("name")))
                {
                    return false;
                }

                auto name = model.at(U("name")).as_string();
                return name.find(U("tinyllama")) != utility::string_t::npos;
            });

            if (!hasTinyLlama)
            {
                RunProcessDetached(L"ollama pull tinyllama");
            }
        }
        catch (...)
        {
            // Не блокируем именно загрузку модели: на этапе классификации это даст fallback на Ask.
            // Важно: ошибки установки/запуска Ollama в EnsureServerAvailable по-прежнему приводят к исключению.
        }
    }
}

namespace CleanAI::Core
{
    OllamaClient::OllamaClient() : OllamaClient(U("http://localhost:11434")) {}

    OllamaClient::OllamaClient(utility::string_t baseUrl) : m_client(std::move(baseUrl)) {}

    void OllamaClient::EnsureServerAvailable()
    {
        if (IsServerAvailable(m_client))
        {
            EnsureModelReady(m_client);
            return;
        }

        if (!IsOllamaInstalled())
        {
            auto installed = TryInstallOllama();
            if (!installed || !IsOllamaInstalled())
            {
                throw winrt::hresult_error(E_FAIL,
                                           L"CleanAI не смог автоматически установить Ollama. "
                                           L"Проверьте, что winget доступен, и повторите запуск.");
            }
        }

        auto started = RunProcessDetached(L"ollama serve");
        if (!started || !WaitUntilServerReady(m_client, 25s))
        {
            throw winrt::hresult_error(E_FAIL,
                                       L"CleanAI не смог автоматически запустить Ollama. "
                                       L"Попробуйте вручную выполнить: ollama serve");
        }

        EnsureModelReady(m_client);
    }

    std::vector<Models::Recommendation> OllamaClient::ClassifyBatch(std::vector<Models::FileItem> const& files)
    {
        std::vector<Models::Recommendation> result;
        result.reserve(files.size());

        for (auto const& file : files)
        {
            Models::Recommendation recommendation{};
            recommendation.type = L"НЕИЗВЕСТНО";
            recommendation.reason = L"Классификация TinyLlama";
            recommendation.confidence = 0.70;
            recommendation.action = Models::RecommendationAction::Ask;

            try
            {
                web::json::value body;
                body[U("model")] = web::json::value::string(U("tinyllama"));

                std::wstring prompt = L"Файл: " + file.path + L", размер: " + std::to_wstring(file.sizeBytes / (1024.0 * 1024.0)) +
                                      L" МБ, не открывался 0 дней. Классифицируй как: КЭШ / ДУБЛИКАТ / ДОКУМЕНТ / МЕДИА / НЕИЗВЕСТНО. "
                                      L"Можно ли удалить без риска? Ответь строго одним словом: УДАЛИТЬ / ОСТАВИТЬ / СПРОСИТЬ.";
                body[U("prompt")] = web::json::value::string(utility::conversions::to_string_t(prompt));
                body[U("stream")] = web::json::value::boolean(false);

                auto response = m_client.request(web::http::methods::POST, U("/api/generate"), body).get();
                if (response.status_code() >= 400)
                {
                    recommendation.reason = L"Fallback: ошибка ответа Ollama";
                    result.push_back(std::move(recommendation));
                    continue;
                }

                auto json = response.extract_json().get();
                auto text = json.has_field(U("response")) ? json[U("response")].as_string() : U("СПРОСИТЬ");

                if (text.find(U("УДАЛИТЬ")) != utility::string_t::npos)
                    recommendation.action = Models::RecommendationAction::Delete;
                else if (text.find(U("ОСТАВИТЬ")) != utility::string_t::npos)
                    recommendation.action = Models::RecommendationAction::Keep;
            }
            catch (...)
            {
                recommendation.reason = L"Fallback: Ollama недоступна или вернула некорректный JSON";
            }

            result.push_back(std::move(recommendation));
        }

        return result;
    }
}
