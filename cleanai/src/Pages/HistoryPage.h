#pragma once

#include <winrt/Microsoft.UI.Xaml.Controls.h>

namespace CleanAI::Pages
{
    struct HistoryPage : winrt::Microsoft::UI::Xaml::Controls::PageT<HistoryPage>
    {
        HistoryPage();
        void OnExport(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&);
    };
}
