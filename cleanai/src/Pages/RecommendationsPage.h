#pragma once

#include "../Models/FileItem.h"
#include "../Models/Recommendation.h"
#include <vector>
#include <winrt/Microsoft.UI.Xaml.Controls.h>

namespace CleanAI::Pages
{
    struct RecommendationsPage : winrt::Microsoft::UI::Xaml::Controls::PageT<RecommendationsPage>
    {
        RecommendationsPage();
        void OnCleanAll(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&);

    private:
        struct RecommendationItem
        {
            Models::FileItem file;
            Models::Recommendation recommendation;
        };

        void LoadDemoRecommendations();
        void PopulateLists();
        winrt::Microsoft::UI::Xaml::Controls::ListViewItem BuildItem(RecommendationItem const& item) const;

        std::vector<RecommendationItem> m_items;
    };
}
