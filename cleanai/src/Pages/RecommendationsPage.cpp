#include "RecommendationsPage.h"

#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Media.h>

namespace
{
    std::wstring ActionText(CleanAI::Models::RecommendationAction action)
    {
        if (action == CleanAI::Models::RecommendationAction::Delete)
        {
            return L"Можно удалить";
        }

        if (action == CleanAI::Models::RecommendationAction::Keep)
        {
            return L"Оставить";
        }

        return L"Нужна проверка";
    }
}

namespace CleanAI::Pages
{
    RecommendationsPage::RecommendationsPage()
    {
        InitializeComponent();
        LoadDemoRecommendations();
        PopulateLists();
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

    void RecommendationsPage::LoadDemoRecommendations()
    {
        m_items = {
            {
                {L"C:\\Users\\Public\\Downloads\\cache.tmp", 745ULL * 1024 * 1024, {}, L".tmp", L"C:\\Users\\Public\\Downloads", Models::FileClass::Cache},
                {L"КЭШ", 0.93, L"Временный файл, давно не использовался", Models::RecommendationAction::Delete}
            },
            {
                {L"C:\\Users\\Public\\Desktop\\budget-2025.xlsx", 4ULL * 1024 * 1024, {}, L".xlsx", L"C:\\Users\\Public\\Desktop", Models::FileClass::DocumentMedia},
                {L"ДОКУМЕНТ", 0.87, L"Похоже на личный документ, лучше спросить", Models::RecommendationAction::Ask}
            },
            {
                {L"C:\\Users\\Public\\Pictures\\family.png", 12ULL * 1024 * 1024, {}, L".png", L"C:\\Users\\Public\\Pictures", Models::FileClass::DocumentMedia},
                {L"МЕДИА", 0.95, L"Личный файл, удалять не рекомендуется", Models::RecommendationAction::Keep}
            }
        };
    }

    void RecommendationsPage::PopulateLists()
    {
        auto deleteList = FindName(L"DeleteList").as<winrt::Microsoft::UI::Xaml::Controls::ListView>();
        auto askList = FindName(L"AskList").as<winrt::Microsoft::UI::Xaml::Controls::ListView>();
        auto keepList = FindName(L"KeepList").as<winrt::Microsoft::UI::Xaml::Controls::ListView>();

        deleteList.Items().Clear();
        askList.Items().Clear();
        keepList.Items().Clear();

        for (auto const& item : m_items)
        {
            auto row = BuildItem(item);
            if (item.recommendation.action == Models::RecommendationAction::Delete)
            {
                deleteList.Items().Append(row);
            }
            else if (item.recommendation.action == Models::RecommendationAction::Keep)
            {
                keepList.Items().Append(row);
            }
            else
            {
                askList.Items().Append(row);
            }
        }
    }

    winrt::Microsoft::UI::Xaml::Controls::ListViewItem RecommendationsPage::BuildItem(RecommendationItem const& item) const
    {
        winrt::Microsoft::UI::Xaml::Controls::ListViewItem row;
        row.Padding(winrt::ThicknessHelper::FromUniformLength(0));

        winrt::Microsoft::UI::Xaml::Controls::Grid grid;
        grid.Padding(winrt::ThicknessHelper::FromLengths(8, 8, 8, 8));

        winrt::Microsoft::UI::Xaml::Controls::StackPanel textStack;
        textStack.Spacing(4);

        winrt::Microsoft::UI::Xaml::Controls::TextBlock title;
        title.Text(winrt::hstring(item.file.path));
        title.TextWrapping(winrt::Microsoft::UI::Xaml::TextWrapping::WrapWholeWords);

        winrt::Microsoft::UI::Xaml::Controls::TextBlock subtitle;
        subtitle.Text(
            winrt::hstring(ActionText(item.recommendation.action) + L" · " +
                           std::to_wstring(item.file.sizeBytes / (1024 * 1024)) + L" МБ · " +
                           item.recommendation.reason));
        subtitle.Opacity(0.82);
        subtitle.TextWrapping(winrt::Microsoft::UI::Xaml::TextWrapping::WrapWholeWords);

        textStack.Children().Append(title);
        textStack.Children().Append(subtitle);
        grid.Children().Append(textStack);

        row.Content(grid);
        return row;
    }
}
