#include "HistoryPage.h"

#include <fstream>

namespace CleanAI::Pages
{
    HistoryPage::HistoryPage()
    {
        InitializeComponent();
    }

    void HistoryPage::OnExport(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
    {
        std::wofstream csv(L"history_export.csv");
        csv << L"дата,путь,действие,размер\n";
        csv << L"2026-02-15 14:30,C:\\Temp\\file.bin,Удалён,1024\n";
    }
}
