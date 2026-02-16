#include "HistoryPage.h"

#include <winrt/Microsoft.UI.Xaml.h>
#include <fstream>
#include <filesystem>
#include <iomanip>
#include <sstream>

namespace
{
    std::wstring EscapeCsv(std::wstring value)
    {
        size_t pos = 0;
        while ((pos = value.find(L'"', pos)) != std::wstring::npos)
        {
            value.insert(pos, 1, L'"');
            pos += 2;
        }

        if (value.find_first_of(L",\n\r") != std::wstring::npos)
        {
            value = L"\"" + value + L"\"";
        }

        return value;
    }

    std::wstring ReadableSize(std::uint64_t bytes)
    {
        constexpr double kb = 1024.0;
        constexpr double mb = kb * 1024.0;
        constexpr double gb = mb * 1024.0;

        std::wstringstream stream;
        stream << std::fixed << std::setprecision(1);
        if (bytes >= static_cast<std::uint64_t>(gb))
        {
            stream << (bytes / gb) << L" ГБ";
        }
        else if (bytes >= static_cast<std::uint64_t>(mb))
        {
            stream << (bytes / mb) << L" МБ";
        }
        else if (bytes >= static_cast<std::uint64_t>(kb))
        {
            stream << (bytes / kb) << L" КБ";
        }
        else
        {
            stream << bytes << L" Б";
        }

        return stream.str();
    }
}

namespace CleanAI::Pages
{
    HistoryPage::HistoryPage()
    {
        InitializeComponent();
        m_history = std::make_shared<Core::HistoryDatabase>();
        m_history->Initialize();
        ReloadHistory();
    }

    void HistoryPage::OnExport(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
    {
        if (m_records.empty())
        {
            return;
        }

        auto exportPath = std::filesystem::current_path() / L"history_export.csv";
        std::wofstream csv(exportPath);
        csv << L"дата,путь,действие,размер,карантин\n";

        for (auto const& record : m_records)
        {
            csv << EscapeCsv(record.timestamp) << L","
                << EscapeCsv(record.path) << L","
                << EscapeCsv(record.action) << L","
                << EscapeCsv(std::to_wstring(record.sizeBytes)) << L","
                << EscapeCsv(record.quarantinePath)
                << L"\n";
        }
    }

    void HistoryPage::ReloadHistory()
    {
        m_records = m_history->LoadHistory();
        auto list = FindName(L"HistoryList").as<winrt::Microsoft::UI::Xaml::Controls::ListView>();
        list.Items().Clear();

        if (m_records.empty())
        {
            Core::HistoryRecord fallback{};
            fallback.timestamp = L"—";
            fallback.path = L"Пока пусто. История появится после первого действия.";
            fallback.action = L"Ожидание";
            fallback.sizeBytes = 0;
            list.Items().Append(BuildRow(fallback));
            return;
        }

        for (auto const& record : m_records)
        {
            list.Items().Append(BuildRow(record));
        }
    }

    winrt::Microsoft::UI::Xaml::Controls::ListViewItem HistoryPage::BuildRow(Core::HistoryRecord const& record) const
    {
        winrt::Microsoft::UI::Xaml::Controls::ListViewItem item;

        winrt::Microsoft::UI::Xaml::Controls::Grid grid;
        grid.Padding(winrt::ThicknessHelper::FromLengths(8, 8, 8, 8));

        winrt::Microsoft::UI::Xaml::Controls::StackPanel stack;
        stack.Spacing(3);

        winrt::Microsoft::UI::Xaml::Controls::TextBlock line1;
        line1.Text(winrt::hstring(record.timestamp + L" · " + record.action));

        winrt::Microsoft::UI::Xaml::Controls::TextBlock line2;
        line2.Text(winrt::hstring(record.path));
        line2.TextWrapping(winrt::Microsoft::UI::Xaml::TextWrapping::WrapWholeWords);

        winrt::Microsoft::UI::Xaml::Controls::TextBlock line3;
        line3.Text(winrt::hstring(L"Размер: " + ReadableSize(record.sizeBytes)));
        line3.Opacity(0.78);

        stack.Children().Append(line1);
        stack.Children().Append(line2);
        stack.Children().Append(line3);
        grid.Children().Append(stack);
        item.Content(grid);
        return item;
    }
}
