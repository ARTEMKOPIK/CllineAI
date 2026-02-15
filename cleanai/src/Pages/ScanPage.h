#pragma once

#include "../Core/DiskScanner.h"
#include <memory>
#include <winrt/Microsoft.UI.Xaml.Controls.h>

namespace CleanAI::Pages
{
    struct ScanPage : winrt::Microsoft::UI::Xaml::Controls::PageT<ScanPage>
    {
        ScanPage();
        winrt::Windows::Foundation::IAsyncAction OnScan(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&);

    private:
        std::shared_ptr<Core::DiskScanner> m_scanner;
    };
}
