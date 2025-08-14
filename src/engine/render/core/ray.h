#pragma once

#include "engine/math/math.h"

template <typename T>
struct TRay
{
    TVec3<T> origin;
    TVec3<T> direction;

    TRay() = default;
    TRay(const TVec3<T>& origin, const TVec3<T>& direction)
        : origin(origin)
        , direction(direction)
    {
    }
    TRay(const TVec3<T>& origin, const TVec3<T>& target)
        : origin(origin)
        , direction(TVec3<T>(target - origin).Normalized())
    {
    }
    TRay(const TRay&) = default;
    TRay& operator=(const TRay&) = default;
    TRay(TRay&&) = default;
    TRay& operator=(TRay&&) = default;

    TVec3<T> At(T t) const
    {
        return origin + direction * t;
    }
};

using Ray = TRay<float>;
using Rayd = TRay<double>;
