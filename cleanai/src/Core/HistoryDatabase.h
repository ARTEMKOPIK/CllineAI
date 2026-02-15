#pragma once

#include "../Models/Recommendation.h"
#include <sqlite3.h>
#include <string>
#include <vector>

namespace CleanAI::Core
{
    struct HistoryRecord
    {
        std::wstring timestamp;
        std::wstring path;
        std::wstring action;
        std::uint64_t sizeBytes{};
        std::wstring quarantinePath;
    };

    class HistoryDatabase
    {
    public:
        HistoryDatabase();
        ~HistoryDatabase();

        void Initialize();
        void SaveAction(HistoryRecord const& record);
        void SaveAiCache(std::wstring const& pathHash, Models::Recommendation const& recommendation);
        std::vector<HistoryRecord> LoadHistory() const;

    private:
        sqlite3* m_db{};
    };
}
