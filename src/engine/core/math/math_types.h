#pragma once

#include <type_traits>

#include "core/math/math_funcs.h"

template <typename T>
struct TVec2
{
    T x = {};
    T y = {};

    TVec2() = default;
    TVec2(const TVec2&) = default;
    TVec2& operator=(const TVec2&) = default;
    TVec2(TVec2&&) = default;
    TVec2& operator=(TVec2&&) = default;

    TVec2(T x, T y) : x(x), y(y) {}

    template <typename TOther>
        requires std::is_floating_point_v<TOther>
    explicit operator TVec2<TOther>() const
    {
        return TVec2<TOther>(static_cast<TOther>(x), static_cast<TOther>(y));
    }

    static TVec2 Zero() { return TVec2(T{}, T{}); }
    static TVec2 One() { return TVec2(T(1), T(1)); }

    TVec2 operator-() const
    {
        return TVec2(-x, -y);
    }

    TVec2& operator+=(const TVec2& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    TVec2 operator+(const TVec2& rhs) const
    {
        return TVec2(*this) += rhs;
    }

    TVec2& operator-=(const TVec2& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    TVec2 operator-(const TVec2& rhs) const
    {
        return TVec2(*this) -= rhs;
    }

    TVec2& operator*=(const T& rhs)
    {
        x *= rhs;
        y *= rhs;
        return *this;
    }

    TVec2 operator*(const T& rhs) const
    {
        return TVec2(*this) *= rhs;
    }

    TVec2& operator/=(const T& rhs)
    {
        x /= rhs;
        y /= rhs;
        return *this;
    }

    TVec2 operator/(const T& rhs) const
    {
        return TVec2(*this) /= rhs;
    }

    bool operator==(const TVec2& rhs) const { return x == rhs.x && y == rhs.y; }
    bool operator!=(const TVec2& rhs) const { return !(*this == rhs); }

    T LengthSqr() const { return x * x + y * y; }
    T Length() const { return Sqrt(LengthSqr()); }

    TVec2& Normalize()
    {
        const T len = Length();
        x /= len;
        y /= len;
        return *this;
    }

    TVec2 GetNormalized() const
    {
        TVec2 norm = *this;
        norm.Normalize();
        return norm;
    }
};

using Vec2 = TVec2<float>;
using Vec2d = TVec2<double>;
using Vec2i = TVec2<int32_t>;
