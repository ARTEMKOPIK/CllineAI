#include "MainWindow.h"

#include "Pages/HistoryPage.h"
#include "Pages/OnboardingPage.h"
#include "Pages/RecommendationsPage.h"
#include "Pages/ScanPage.h"
#include "Pages/TreeMapPage.h"
#include <winrt/Microsoft.UI.Xaml.Controls.h>

namespace CleanAI
{
    MainWindow::MainWindow(bool onboardingCompleted)
    {
        InitializeComponent();
        NavigateInitial(onboardingCompleted);
    }

    void MainWindow::NavigateInitial(bool onboardingCompleted)
    {
        auto frame = this->FindName(L"ContentFrame").as<winrt::Microsoft::UI::Xaml::Controls::Frame>();
        if (!onboardingCompleted)
        {
            frame.Navigate(winrt::xaml_typename<Pages::OnboardingPage>());
            return;
        }

        frame.Navigate(winrt::xaml_typename<Pages::ScanPage>());
    }

    void MainWindow::OnNavigationSelectionChanged(winrt::Microsoft::UI::Xaml::Controls::NavigationView const&,
                                                   winrt::Microsoft::UI::Xaml::Controls::NavigationViewSelectionChangedEventArgs const& args)
    {
        auto item = args.SelectedItemContainer();
        auto tag = winrt::unbox_value<winrt::hstring>(item.Tag());
        auto frame = this->FindName(L"ContentFrame").as<winrt::Microsoft::UI::Xaml::Controls::Frame>();

        if (tag == L"scan") frame.Navigate(winrt::xaml_typename<Pages::ScanPage>());
        if (tag == L"treemap") frame.Navigate(winrt::xaml_typename<Pages::TreeMapPage>());
        if (tag == L"recommendations") frame.Navigate(winrt::xaml_typename<Pages::RecommendationsPage>());
        if (tag == L"history") frame.Navigate(winrt::xaml_typename<Pages::HistoryPage>());
    }
}
