#pragma once

#include <winrt/Microsoft.UI.Xaml.Controls.h>

namespace CleanAI::Pages
{
    struct RecommendationsPage : winrt::Microsoft::UI::Xaml::Controls::PageT<RecommendationsPage>
    {
        RecommendationsPage();
        void OnCleanAll(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&);
    };
}
