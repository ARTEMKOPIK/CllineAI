#pragma once

#include <string>

namespace CleanAI::Models
{
    enum class RecommendationAction
    {
        Delete,
        Ask,
        Keep
    };

    struct Recommendation
    {
        std::wstring type;
        double confidence{};
        std::wstring reason;
        RecommendationAction action{RecommendationAction::Ask};
    };
}
