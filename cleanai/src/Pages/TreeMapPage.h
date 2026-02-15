#pragma once

#include "../Core/TreemapLayout.h"
#include <winrt/Microsoft.UI.Xaml.Controls.h>

namespace CleanAI::Pages
{
    struct TreeMapPage : winrt::Microsoft::UI::Xaml::Controls::PageT<TreeMapPage>
    {
        TreeMapPage();
        void RenderDemo();
    };
}
