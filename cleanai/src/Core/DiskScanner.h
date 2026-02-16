#pragma once

#include "../Models/FileItem.h"
#include <atomic>
#include <functional>
#include <mutex>
#include <vector>

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

        std::vector<Models::FileItem> Scan(std::wstring const& root, ProgressCallback callback);
        static std::vector<std::wstring> DetectLocalDrives();

    public:
        static bool IsSystemDirectoryPathForSkip(std::wstring const& path);

    private:
        bool ShouldSkipDirectory(std::wstring const& path) const;
        std::mutex m_mutex;
    };
}
