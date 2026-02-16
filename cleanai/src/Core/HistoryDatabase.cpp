#include "HistoryDatabase.h"

#include <filesystem>

namespace CleanAI::Core
{
    HistoryDatabase::HistoryDatabase()
    {
        wchar_t* localAppData{};
        size_t len{};
        _wdupenv_s(&localAppData, &len, L"LOCALAPPDATA");
        auto const appDataPath =
            (localAppData != nullptr && len > 0)
                ? std::filesystem::path(localAppData)
                : std::filesystem::temp_directory_path();

        auto dbPath = appDataPath / L"CleanAI" / L"cleanai.db";
        std::filesystem::create_directories(dbPath.parent_path());
        if (sqlite3_open16(dbPath.c_str(), &m_db) != SQLITE_OK)
        {
            if (m_db)
            {
                sqlite3_close(m_db);
                m_db = nullptr;
            }
        }
        free(localAppData);
    }

    HistoryDatabase::~HistoryDatabase()
    {
        if (m_db)
        {
            sqlite3_close(m_db);
        }
    }

    void HistoryDatabase::Initialize()
    {
        if (!m_db)
        {
            return;
        }

        char const* createActions =
            "CREATE TABLE IF NOT EXISTS actions ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "timestamp TEXT,"
            "path TEXT,"
            "action TEXT,"
            "size INTEGER,"
            "quarantine_path TEXT);";

        char const* createAiCache =
            "CREATE TABLE IF NOT EXISTS ai_cache ("
            "path_hash TEXT PRIMARY KEY,"
            "recommendation TEXT,"
            "confidence REAL,"
            "timestamp TEXT);";

        sqlite3_exec(m_db, createActions, nullptr, nullptr, nullptr);
        sqlite3_exec(m_db, createAiCache, nullptr, nullptr, nullptr);
    }

    void HistoryDatabase::SaveAction(HistoryRecord const& record)
    {
        if (!m_db)
        {
            return;
        }

        sqlite3_stmt* stmt{};
        sqlite3_prepare16_v2(m_db, L"INSERT INTO actions(timestamp,path,action,size,quarantine_path) VALUES(?,?,?,?,?)", -1, &stmt, nullptr);
        sqlite3_bind_text16(stmt, 1, record.timestamp.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text16(stmt, 2, record.path.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text16(stmt, 3, record.action.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int64(stmt, 4, static_cast<sqlite3_int64>(record.sizeBytes));
        sqlite3_bind_text16(stmt, 5, record.quarantinePath.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    void HistoryDatabase::SaveAiCache(std::wstring const& pathHash, Models::Recommendation const& recommendation)
    {
        if (!m_db)
        {
            return;
        }

        sqlite3_stmt* stmt{};
        sqlite3_prepare16_v2(m_db, L"INSERT OR REPLACE INTO ai_cache(path_hash,recommendation,confidence,timestamp) VALUES(?,?,?,datetime('now'))", -1, &stmt, nullptr);
        sqlite3_bind_text16(stmt, 1, pathHash.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text16(stmt, 2, recommendation.reason.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_double(stmt, 3, recommendation.confidence);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    std::vector<HistoryRecord> HistoryDatabase::LoadHistory() const
    {
        std::vector<HistoryRecord> result;
        if (!m_db)
        {
            return result;
        }

        sqlite3_stmt* stmt{};
        sqlite3_prepare16_v2(m_db, L"SELECT timestamp,path,action,size,quarantine_path FROM actions ORDER BY id DESC", -1, &stmt, nullptr);

        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            HistoryRecord record{};
            record.timestamp = static_cast<wchar_t const*>(sqlite3_column_text16(stmt, 0));
            record.path = static_cast<wchar_t const*>(sqlite3_column_text16(stmt, 1));
            record.action = static_cast<wchar_t const*>(sqlite3_column_text16(stmt, 2));
            record.sizeBytes = static_cast<std::uint64_t>(sqlite3_column_int64(stmt, 3));
            record.quarantinePath = static_cast<wchar_t const*>(sqlite3_column_text16(stmt, 4));
            result.push_back(std::move(record));
        }

        sqlite3_finalize(stmt);
        return result;
    }
}
