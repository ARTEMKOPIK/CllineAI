#pragma once

#include "../Models/FileItem.h"
#include "../Models/Recommendation.h"
#include <cpprest/http_client.h>

namespace CleanAI::Core
{
    class OllamaClient
    {
    public:
        OllamaClient();
        explicit OllamaClient(utility::string_t baseUrl);
        void EnsureServerAvailable();
        std::vector<Models::Recommendation> ClassifyBatch(std::vector<Models::FileItem> const& files);

    private:
        web::http::client::http_client m_client;
    };
}
