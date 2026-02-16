#pragma once

#include "../Core/HistoryDatabase.h"
#include <memory>
#include <vector>
#include <winrt/Microsoft.UI.Xaml.Controls.h>

namespace CleanAI::Pages
{
    struct HistoryPage : winrt::Microsoft::UI::Xaml::Controls::PageT<HistoryPage>
    {
        HistoryPage();
        void OnExport(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&);

    private:
        void ReloadHistory();
        winrt::Microsoft::UI::Xaml::Controls::ListViewItem BuildRow(Core::HistoryRecord const& record) const;

        std::shared_ptr<Core::HistoryDatabase> m_history;
        std::vector<Core::HistoryRecord> m_records;
    };
}
