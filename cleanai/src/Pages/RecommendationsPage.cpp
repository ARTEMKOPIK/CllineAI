#include "RecommendationsPage.h"

#include <winrt/Microsoft.UI.Xaml.Controls.h>

namespace CleanAI::Pages
{
    RecommendationsPage::RecommendationsPage()
    {
        InitializeComponent();
    }

    void RecommendationsPage::OnCleanAll(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
    {
        winrt::Microsoft::UI::Xaml::Controls::ContentDialog dialog;
        dialog.Title(winrt::box_value(L"Подтверждение"));
        dialog.Content(winrt::box_value(L"Удалить 47.2 ГБ безопасных файлов?"));
        dialog.PrimaryButtonText(L"Удалить");
        dialog.CloseButtonText(L"Отмена");
        dialog.XamlRoot(XamlRoot());
        dialog.ShowAsync();
    }
}
