#include "DiskScanner.h"

#include <filesystem>
#include <windows.h>

using namespace std::chrono_literals;

namespace CleanAI::Core
{
    winrt::Windows::Foundation::IAsyncOperation<std::vector<Models::FileItem>> DiskScanner::ScanAsync(std::wstring root, ProgressCallback callback)
    {
        co_await winrt::resume_background();

        std::vector<Models::FileItem> files;
        ScanProgress progress{};
        auto lastUpdate = std::chrono::steady_clock::now();

        auto iterator = std::filesystem::recursive_directory_iterator(
            root,
            std::filesystem::directory_options::skip_permission_denied);

        auto end = std::filesystem::recursive_directory_iterator();
        for (; iterator != end; ++iterator)
        {
            try
            {
                auto const& entry = *iterator;

                if (entry.is_directory() && ShouldSkipDirectory(entry.path().wstring()))
                {
                    iterator.disable_recursion_pending();
                    continue;
                }

                if (!entry.is_regular_file())
                {
                    continue;
                }

                Models::FileItem item{};
                item.path = entry.path().wstring();
                item.sizeBytes = entry.file_size();
                item.extension = entry.path().extension().wstring();
                item.parentFolder = entry.path().parent_path().wstring();
                files.push_back(std::move(item));

                progress.currentPath = entry.path().wstring();
                progress.filesCount++;
                progress.totalBytes += files.back().sizeBytes;

                if (std::chrono::steady_clock::now() - lastUpdate >= 1s)
                {
                    callback(progress);
                    lastUpdate = std::chrono::steady_clock::now();
                }
            }
            catch (...)
            {
                // Пропускаем недоступные или удалённые в процессе сканирования элементы,
                // но продолжаем сканирование остальных файлов.
                continue;
            }
        }

        callback(progress);
        co_return files;
    }

    std::vector<std::wstring> DiskScanner::DetectLocalDrives()
    {
        std::vector<std::wstring> drives;
        wchar_t buffer[256]{};
        if (GetLogicalDriveStringsW(256, buffer) > 0)
        {
            for (wchar_t* p = buffer; *p; p += wcslen(p) + 1)
            {
                if (GetDriveTypeW(p) == DRIVE_FIXED)
                {
                    drives.emplace_back(p);
                }
            }
        }
        return drives;
    }

    bool DiskScanner::ShouldSkipDirectory(std::wstring const& path) const
    {
        static constexpr std::wstring_view blocked[] = {
            L"$Recycle.Bin",
            L"System Volume Information",
            L"Windows",
            L"Program Files"
        };

        for (auto const& item : blocked)
        {
            if (path.find(item) != std::wstring::npos)
            {
                return true;
            }
        }

        return false;
    }
}
