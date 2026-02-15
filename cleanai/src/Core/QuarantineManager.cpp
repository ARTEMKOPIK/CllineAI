#include "QuarantineManager.h"

#include <Windows.h>
#include <chrono>
#include <stdexcept>
#include <format>

namespace CleanAI::Core
{
    QuarantineManager::QuarantineManager()
    {
        wchar_t* localAppData{};
        size_t len{};
        _wdupenv_s(&localAppData, &len, L"LOCALAPPDATA");
        m_quarantineRoot = std::filesystem::path(localAppData) / L"CleanAI" / L"quarantine";
        std::filesystem::create_directories(m_quarantineRoot);
        free(localAppData);
    }

    winrt::Windows::Foundation::IAsyncOperation<std::wstring> QuarantineManager::MoveToQuarantineAsync(std::wstring const& rootDrive, std::wstring const& filePath)
    {
        co_await winrt::resume_background();

        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        std::tm utc{};
        gmtime_s(&utc, &t);
        wchar_t buffer[32]{};
        wcsftime(buffer, 32, L"%Y%m%d_%H%M%S", &utc);

        auto sessionPath = m_quarantineRoot / buffer;
        auto relative = std::filesystem::relative(filePath, rootDrive);
        auto target = sessionPath / relative;
        std::filesystem::create_directories(target.parent_path());

        if (!MoveFileExW(filePath.c_str(), target.wstring().c_str(), MOVEFILE_COPY_ALLOWED | MOVEFILE_WRITE_THROUGH))
        {
            throw std::runtime_error("Failed to move file into quarantine");
        }
        co_return target.wstring();
    }

    winrt::Windows::Foundation::IAsyncAction QuarantineManager::RestoreAsync(std::wstring const& quarantinePath, std::wstring const& originalPath)
    {
        co_await winrt::resume_background();

        std::filesystem::create_directories(std::filesystem::path(originalPath).parent_path());
        MoveFileExW(quarantinePath.c_str(), originalPath.c_str(), MOVEFILE_COPY_ALLOWED | MOVEFILE_WRITE_THROUGH);
    }

    winrt::Windows::Foundation::IAsyncAction QuarantineManager::CleanupOldAsync()
    {
        co_await winrt::resume_background();

        auto cutoff = std::chrono::file_clock::now() - std::chrono::hours(24 * 30);
        for (auto const& entry : std::filesystem::directory_iterator(m_quarantineRoot))
        {
            if (entry.is_directory() && entry.last_write_time() < cutoff)
            {
                std::filesystem::remove_all(entry.path());
            }
        }
    }

    std::uintmax_t QuarantineManager::GetQuarantineSizeBytes() const
    {
        std::uintmax_t total{};
        for (auto const& entry : std::filesystem::recursive_directory_iterator(m_quarantineRoot, std::filesystem::directory_options::skip_permission_denied))
        {
            if (entry.is_regular_file())
            {
                total += entry.file_size();
            }
        }
        return total;
    }
}
