#include "TreeMapPage.h"

#include "../Models/FileItem.h"
#include <sstream>
#include <winrt/Microsoft.UI.Xaml.Media.h>
#include <winrt/Microsoft.UI.Xaml.Shapes.h>

namespace CleanAI::Pages
{
    TreeMapPage::TreeMapPage()
    {
        InitializeComponent();
        RenderDemo();
    }

    void TreeMapPage::RenderDemo()
    {
        std::vector<Models::FileItem> sample{
            {L"C:\\Temp\\cache.bin", 450 * 1024 * 1024, {}, L".bin", L"C:\\Temp", Models::FileClass::Cache},
            {L"C:\\Users\\Doc.pdf", 120 * 1024 * 1024, {}, L".pdf", L"C:\\Users", Models::FileClass::DocumentMedia}
        };

        Core::TreemapLayout layout;
        auto rects = layout.BuildSquarified(sample, 800, 500);

        auto canvas = FindName(L"TreemapCanvas").as<winrt::Microsoft::UI::Xaml::Controls::Canvas>();
        canvas.Children().Clear();

        for (auto const& item : rects)
        {
            auto color = winrt::Windows::UI::ColorHelper::FromArgb(255, 239, 68, 68);
            if (item.file.classification == Models::FileClass::DocumentMedia)
            {
                color = winrt::Windows::UI::ColorHelper::FromArgb(255, 59, 130, 246);
            }

            std::wstringstream tooltip;
            tooltip << item.file.path << L" — " << (item.file.sizeBytes / (1024 * 1024)) << L" МБ";

            winrt::Microsoft::UI::Xaml::Shapes::Rectangle rectangle;
            rectangle.Width(item.width);
            rectangle.Height(item.height);
            rectangle.RadiusX(4);
            rectangle.RadiusY(4);
            rectangle.Fill(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush(color));
            winrt::Microsoft::UI::Xaml::Controls::ToolTipService::SetToolTip(rectangle, winrt::box_value(winrt::hstring(tooltip.str())));
            winrt::Microsoft::UI::Xaml::Controls::Canvas::SetLeft(rectangle, item.x);
            winrt::Microsoft::UI::Xaml::Controls::Canvas::SetTop(rectangle, item.y);
            canvas.Children().Append(rectangle);
        }
    }
}
