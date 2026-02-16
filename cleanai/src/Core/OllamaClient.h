#pragma once

#include "../Models/FileItem.h"
#include "../Models/Recommendation.h"
#include <cpprest/http_client.h>
#include <winrt/Windows.Foundation.h>

namespace CleanAI::Core
{
    class OllamaClient
    {
    public:
        OllamaClient();
        explicit OllamaClient(utility::string_t baseUrl);
        winrt::Windows::Foundation::IAsyncAction EnsureServerAvailableAsync();
        winrt::Windows::Foundation::IAsyncOperation<std::vector<Models::Recommendation>> ClassifyBatchAsync(std::vector<Models::FileItem> const& files);

    private:
        web::http::client::http_client m_client;
    };
}
