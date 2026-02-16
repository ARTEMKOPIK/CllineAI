#pragma once

#include "../Models/FileItem.h"
#include <atomic>
#include <functional>
#include <mutex>
#include <vector>
#include <winrt/Windows.Foundation.h>

namespace CleanAI::Core
{
    struct ScanProgress
    {
        std::wstring currentPath;
        std::uint64_t filesCount{};
        std::uint64_t totalBytes{};
    };

    class DiskScanner
    {
    public:
        using ProgressCallback = std::function<void(ScanProgress const&)>;

        winrt::Windows::Foundation::IAsyncOperation<std::vector<Models::FileItem>> ScanAsync(std::wstring root, ProgressCallback callback);
        static std::vector<std::wstring> DetectLocalDrives();

    public:
        static bool IsSystemDirectoryPathForSkip(std::wstring const& path);

    private:
        bool ShouldSkipDirectory(std::wstring const& path) const;
        std::mutex m_mutex;
    };
}
