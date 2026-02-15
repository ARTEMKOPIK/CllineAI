#pragma once

#include <chrono>
#include <string>

namespace CleanAI::Models
{
    enum class FileClass
    {
        Cache,
        Duplicate,
        DocumentMedia,
        Unknown
    };

    struct FileItem
    {
        std::wstring path;
        std::uintmax_t sizeBytes{};
        std::chrono::system_clock::time_point lastAccess{};
        std::wstring extension;
        std::wstring parentFolder;
        FileClass classification{FileClass::Unknown};
    };
}
