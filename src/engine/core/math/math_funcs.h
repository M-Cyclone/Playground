#pragma once

#include <type_traits>
#include <algorithm>
#include <cmath>

#include "core/macro_define.h"

static constexpr float kEpsilon = 1e-4f;
static constexpr double kEpsilonD = 1e-8;

template <typename T>
struct TFloatConstants
{
    static constexpr T GetEpsilon()
    {
        if constexpr (std::is_same_v<T, float>)
            return kEpsilon;
        else if constexpr (std::is_same_v<T, double>)
            return kEpsilonD;
        else
            static_assert(std::is_floating_point_v<T>, "Unsupported floating point type");
    }
};

template <typename T>
ENGINE_FORCE_INLINE T Sqrt(T value)
{
    return std::sqrt(value);
}
