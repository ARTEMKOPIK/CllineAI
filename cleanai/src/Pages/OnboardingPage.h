#pragma once

#include <winrt/Microsoft.UI.Xaml.Controls.h>

namespace CleanAI::Pages
{
    struct OnboardingPage : winrt::Microsoft::UI::Xaml::Controls::PageT<OnboardingPage>
    {
        OnboardingPage();
        void OnBack(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&);
        void OnNext(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&);

    private:
        int m_index{0};
        void UpdateScreen();
    };
}
