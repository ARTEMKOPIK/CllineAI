#pragma once

#include "../Models/FileItem.h"
#include <vector>

namespace CleanAI::Core
{
    struct Rect
    {
        double x{};
        double y{};
        double width{};
        double height{};
        Models::FileItem file;
    };

    class TreemapLayout
    {
    public:
        std::vector<Rect> BuildSquarified(std::vector<Models::FileItem> const& files, double width, double height) const;

    private:
        static double Score(std::vector<double> const& row, double shortSide);
    };
}
