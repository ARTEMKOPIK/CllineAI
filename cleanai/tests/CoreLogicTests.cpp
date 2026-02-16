#include "../src/Core/DiskScanner.h"
#include "../src/Core/OllamaClient.h"
#include "../src/Models/FileItem.h"

#include <iostream>
#include <vector>
#include <winrt/base.h>

namespace
{
    int AssertTrue(bool condition, char const* message)
    {
        if (!condition)
        {
            std::cerr << "[FAIL] " << message << std::endl;
            return 1;
        }

        std::cout << "[PASS] " << message << std::endl;
        return 0;
    }

    int TestShouldSkipDirectory()
    {
        int failures = 0;
        using CleanAI::Core::DiskScanner;

        failures += AssertTrue(DiskScanner::IsSystemDirectoryPathForSkip(L"C:\\Windows"), "Root Windows directory is skipped");
        failures += AssertTrue(DiskScanner::IsSystemDirectoryPathForSkip(L"D:\\Program Files\\App"), "Root Program Files directory is skipped");
        failures += AssertTrue(DiskScanner::IsSystemDirectoryPathForSkip(L"E:\\$Recycle.Bin\\S-1-5"), "$Recycle.Bin is skipped");

        failures += AssertTrue(!DiskScanner::IsSystemDirectoryPathForSkip(L"D:\\Users\\John\\MyWindowsNotes"), "Substring-only match should not skip user folder");
        failures += AssertTrue(!DiskScanner::IsSystemDirectoryPathForSkip(L"D:\\Data\\Program Files Backup"), "Substring-only Program Files match should not skip");
        failures += AssertTrue(!DiskScanner::IsSystemDirectoryPathForSkip(L"D:\\Projects\\System Volume Information Docs"), "Substring-only System Volume Information match should not skip");

        return failures;
    }

    int TestClassifyBatchFallback()
    {
        int failures = 0;

        CleanAI::Core::OllamaClient client(U("http://127.0.0.1:1"));
        std::vector<CleanAI::Models::FileItem> files(1);
        files[0].path = L"C:\\Temp\\demo.tmp";
        files[0].sizeBytes = 1024;

        auto result = client.ClassifyBatchAsync(files).get();
        failures += AssertTrue(result.size() == 1, "Fallback returns recommendation for each input file");
        failures += AssertTrue(result[0].action == CleanAI::Models::RecommendationAction::Ask, "On network error recommendation falls back to Ask");

        return failures;
    }
}

int main()
{
    winrt::init_apartment();

    int failures = 0;
    failures += TestShouldSkipDirectory();
    failures += TestClassifyBatchFallback();

    if (failures == 0)
    {
        std::cout << "All Core logic tests passed." << std::endl;
        return 0;
    }

    std::cerr << "Core logic tests failed: " << failures << std::endl;
    return 1;
}
