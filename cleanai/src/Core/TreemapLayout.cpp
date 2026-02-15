#include "TreemapLayout.h"

#include <algorithm>
#include <numeric>

namespace CleanAI::Core
{
    double TreemapLayout::Score(std::vector<double> const& row, double shortSide)
    {
        if (row.empty()) return 0.0;

        auto minMax = std::minmax_element(row.begin(), row.end());
        double sum = std::accumulate(row.begin(), row.end(), 0.0);
        double squared = shortSide * shortSide;
        return std::max((squared * (*minMax.second)) / (sum * sum), (sum * sum) / (squared * (*minMax.first)));
    }

    std::vector<Rect> TreemapLayout::BuildSquarified(std::vector<Models::FileItem> const& files, double width, double height) const
    {
        std::vector<Rect> result;
        if (files.empty() || width <= 0 || height <= 0) return result;

        std::vector<Models::FileItem> sorted = files;
        std::ranges::sort(sorted, [](auto const& a, auto const& b) { return a.sizeBytes > b.sizeBytes; });

        double total = 0.0;
        for (auto const& f : sorted) total += static_cast<double>(f.sizeBytes);

        double x = 0, y = 0, w = width, h = height;
        for (auto const& file : sorted)
        {
            double ratio = static_cast<double>(file.sizeBytes) / total;
            double area = ratio * width * height;
            double rw = std::max(8.0, std::sqrt(area));
            double rh = std::max(8.0, area / rw);

            if (x + rw > width)
            {
                x = 0;
                y += rh;
            }

            if (y + rh > height) break;

            result.push_back(Rect{ x, y, rw, rh, file });
            x += rw;
        }

        return result;
    }
}
