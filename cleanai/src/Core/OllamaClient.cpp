#include "OllamaClient.h"

#include <chrono>

namespace CleanAI::Core
{
    OllamaClient::OllamaClient() : m_client(U("http://localhost:11434")) {}

    winrt::Windows::Foundation::IAsyncAction OllamaClient::EnsureServerAvailableAsync()
    {
        co_await winrt::resume_background();

        auto response = m_client.request(web::http::methods::GET, U("/api/tags")).get();
        if (response.status_code() >= 400)
        {
            throw winrt::hresult_error(E_FAIL, L"Ошибка: сервер Ollama не запущен. Запустите ollama serve и перезапустите приложение");
        }
    }

    winrt::Windows::Foundation::IAsyncOperation<std::vector<Models::Recommendation>> OllamaClient::ClassifyBatchAsync(std::vector<Models::FileItem> const& files)
    {
        co_await winrt::resume_background();

        std::vector<Models::Recommendation> result;
        result.reserve(files.size());

        for (auto const& file : files)
        {
            web::json::value body;
            body[U("model")] = web::json::value::string(U("tinyllama"));

            std::wstring prompt = L"Файл: " + file.path + L", размер: " + std::to_wstring(file.sizeBytes / (1024.0 * 1024.0)) +
                                  L" МБ, не открывался 0 дней. Классифицируй как: КЭШ / ДУБЛИКАТ / ДОКУМЕНТ / МЕДИА / НЕИЗВЕСТНО. "
                                  L"Можно ли удалить без риска? Ответь строго одним словом: УДАЛИТЬ / ОСТАВИТЬ / СПРОСИТЬ.";
            body[U("prompt")] = web::json::value::string(utility::conversions::to_string_t(prompt));
            body[U("stream")] = web::json::value::boolean(false);

            auto response = m_client.request(web::http::methods::POST, U("/api/generate"), body).get();
            auto json = response.extract_json().get();
            auto text = json.has_field(U("response")) ? json[U("response")].as_string() : U("СПРОСИТЬ");

            Models::Recommendation recommendation{};
            recommendation.type = L"НЕИЗВЕСТНО";
            recommendation.reason = L"Классификация tinyllama";
            recommendation.confidence = 0.70;

            if (text.find(U("УДАЛИТЬ")) != utility::string_t::npos)
                recommendation.action = Models::RecommendationAction::Delete;
            else if (text.find(U("ОСТАВИТЬ")) != utility::string_t::npos)
                recommendation.action = Models::RecommendationAction::Keep;
            else
                recommendation.action = Models::RecommendationAction::Ask;

            result.push_back(std::move(recommendation));
        }

        co_return result;
    }
}
