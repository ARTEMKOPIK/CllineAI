#include "TreemapLayout.h"

#include <algorithm>
#include <numeric>
#include <limits>

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
        if (files.empty() || width <= 0 || height <= 0)
        {
            return result;
        }

        std::vector<Models::FileItem> sorted = files;
        std::ranges::sort(sorted, [](auto const& a, auto const& b)
        {
            return a.sizeBytes > b.sizeBytes;
        });

        double total = 0.0;
        for (auto const& file : sorted)
        {
            total += static_cast<double>(file.sizeBytes);
        }

        if (total <= 0.0)
        {
            return result;
        }

        struct WeightedFile
        {
            Models::FileItem file;
            double area{};
        };

        std::vector<WeightedFile> weighted;
        weighted.reserve(sorted.size());
        auto const totalArea = width * height;

        for (auto const& file : sorted)
        {
            auto area = (static_cast<double>(file.sizeBytes) / total) * totalArea;
            if (area > 0.0)
            {
                weighted.push_back(WeightedFile{file, area});
            }
        }

        if (weighted.empty())
        {
            return result;
        }

        double left = 0.0;
        double top = 0.0;
        double remainingWidth = width;
        double remainingHeight = height;

        std::vector<WeightedFile> row;
        std::vector<double> rowAreas;
        auto index = size_t{0};

        auto flushRow = [&](bool horizontal)
        {
            if (row.empty())
            {
                return;
            }

            auto rowTotalArea = std::accumulate(rowAreas.begin(), rowAreas.end(), 0.0);
            if (rowTotalArea <= 0.0)
            {
                row.clear();
                rowAreas.clear();
                return;
            }

            if (horizontal)
            {
                auto rowHeight = rowTotalArea / remainingWidth;
                double cursorX = left;
                for (auto const& item : row)
                {
                    auto rectWidth = item.area / rowHeight;
                    result.push_back(Rect{cursorX, top, rectWidth, rowHeight, item.file});
                    cursorX += rectWidth;
                }

                top += rowHeight;
                remainingHeight -= rowHeight;
            }
            else
            {
                auto rowWidth = rowTotalArea / remainingHeight;
                double cursorY = top;
                for (auto const& item : row)
                {
                    auto rectHeight = item.area / rowWidth;
                    result.push_back(Rect{left, cursorY, rowWidth, rectHeight, item.file});
                    cursorY += rectHeight;
                }

                left += rowWidth;
                remainingWidth -= rowWidth;
            }

            row.clear();
            rowAreas.clear();
        };

        while (index < weighted.size())
        {
            if (remainingWidth <= 0.0 || remainingHeight <= 0.0)
            {
                break;
            }

            bool horizontal = remainingWidth >= remainingHeight;
            auto shortSide = horizontal ? remainingHeight : remainingWidth;
            if (shortSide <= 0.0)
            {
                break;
            }

            auto const& candidate = weighted[index];
            auto rowScore = row.empty() ? std::numeric_limits<double>::infinity() : Score(rowAreas, shortSide);
            auto nextAreas = rowAreas;
            nextAreas.push_back(candidate.area);
            auto nextScore = Score(nextAreas, shortSide);

            if (row.empty() || nextScore <= rowScore)
            {
                row.push_back(candidate);
                rowAreas = std::move(nextAreas);
                ++index;
                continue;
            }

            flushRow(horizontal);
        }

        if (!row.empty())
        {
            bool horizontal = remainingWidth >= remainingHeight;
            flushRow(horizontal);
        }

        return result;
    }
}
