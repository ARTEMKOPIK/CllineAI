#include "TreeMapPage.h"

#include "../Models/FileItem.h"
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
            winrt::Microsoft::UI::Xaml::Shapes::Rectangle rectangle;
            rectangle.Width(item.width);
            rectangle.Height(item.height);
            rectangle.RadiusX(4);
            rectangle.RadiusY(4);
            rectangle.Fill(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush(winrt::Windows::UI::ColorHelper::FromArgb(255, 239, 68, 68)));
            winrt::Microsoft::UI::Xaml::Controls::ToolTipService::SetToolTip(rectangle, winrt::box_value(winrt::hstring(item.file.path + L" — 450 МБ")));
            winrt::Microsoft::UI::Xaml::Controls::Canvas::SetLeft(rectangle, item.x);
            winrt::Microsoft::UI::Xaml::Controls::Canvas::SetTop(rectangle, item.y);
            canvas.Children().Append(rectangle);
        }
    }
}
