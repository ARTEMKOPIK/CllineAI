#pragma once

#include <winrt/Microsoft.UI.Xaml.h>

namespace CleanAI
{
    struct MainWindow : winrt::Microsoft::UI::Xaml::WindowT<MainWindow>
    {
        MainWindow(bool onboardingCompleted);
        void OnNavigationSelectionChanged(winrt::Microsoft::UI::Xaml::Controls::NavigationView const&, winrt::Microsoft::UI::Xaml::Controls::NavigationViewSelectionChangedEventArgs const&);

    private:
        void NavigateInitial(bool onboardingCompleted);
    };
}
