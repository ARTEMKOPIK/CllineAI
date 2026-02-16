#include "ScanPage.h"

namespace CleanAI::Pages
{
    ScanPage::ScanPage()
    {
        InitializeComponent();
        m_scanner = std::make_shared<Core::DiskScanner>();

        auto combo = FindName(L"DriveCombo").as<winrt::Microsoft::UI::Xaml::Controls::ComboBox>();
        for (auto const& drive : Core::DiskScanner::DetectLocalDrives())
        {
            combo.Items().Append(winrt::box_value(drive));
        }
        if (combo.Items().Size() > 0) combo.SelectedIndex(0);
    }

    winrt::Windows::Foundation::IAsyncAction ScanPage::OnScan(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
    {
        auto combo = FindName(L"DriveCombo").as<winrt::Microsoft::UI::Xaml::Controls::ComboBox>();
        if (!combo.SelectedItem()) co_return;

        auto selectedDrive = winrt::unbox_value<winrt::hstring>(combo.SelectedItem());
        std::wstring root = selectedDrive.c_str();

        auto files = co_await m_scanner->ScanAsync(root, [this](Core::ScanProgress const& progress)
        {
            DispatcherQueue().TryEnqueue([this, progress]()
            {
                FindName(L"CurrentPathText").as<winrt::Microsoft::UI::Xaml::Controls::TextBlock>().Text(L"Текущий путь: " + winrt::hstring(progress.currentPath));
                FindName(L"FilesCountText").as<winrt::Microsoft::UI::Xaml::Controls::TextBlock>().Text(L"Файлов: " + winrt::to_hstring(progress.filesCount));
                FindName(L"SizeText").as<winrt::Microsoft::UI::Xaml::Controls::TextBlock>().Text(L"Размер: " + winrt::to_hstring(progress.totalBytes) + L" Б");
            });
        });

        (void)files;
    }
}
