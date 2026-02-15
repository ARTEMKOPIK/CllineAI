#pragma once

#include <filesystem>
#include <string>
#include <winrt/Windows.Foundation.h>

namespace CleanAI::Core
{
    class QuarantineManager
    {
    public:
        QuarantineManager();

        winrt::Windows::Foundation::IAsyncOperation<std::wstring> MoveToQuarantineAsync(std::wstring const& rootDrive, std::wstring const& filePath);
        winrt::Windows::Foundation::IAsyncAction RestoreAsync(std::wstring const& quarantinePath, std::wstring const& originalPath);
        winrt::Windows::Foundation::IAsyncAction CleanupOldAsync();
        std::uintmax_t GetQuarantineSizeBytes() const;

    private:
        std::filesystem::path m_quarantineRoot;
    };
}
