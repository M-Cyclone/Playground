#pragma once

#include "engine/math/math_funcs.h"

#include <type_traits>

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

    TVec2& Normalized()
    {
        const T len = Length();
        x /= len;
        y /= len;
        return *this;
    }

    TVec2 Normalize() const
    {
        TVec2 norm = *this;
        norm.Normalized();
        return norm;
    }

    T Dot(const TVec2& rhs) const
    {
        return x * rhs.x + y * rhs.y;
    }
};

using Vec2 = TVec2<float>;
using Vec2d = TVec2<double>;
using Vec2i = TVec2<int32_t>;

// We use left-up-forward coordinate system.

template <typename T>
struct TVec3
{
    T x = {};
    T y = {};
    T z = {};

    TVec3() = default;
    TVec3(const TVec3&) = default;
    TVec3& operator=(const TVec3&) = default;
    TVec3(TVec3&&) = default;
    TVec3& operator=(TVec3&&) = default;

    TVec3(T x, T y, T z) : x(x), y(y), z(z) {}

    template <typename TOther>
    explicit operator TVec3<TOther>() const
    {
        return TVec3<TOther>(static_cast<TOther>(x), static_cast<TOther>(y), static_cast<TOther>(z));
    }

    static TVec3 Zero() { return TVec3(T{}, T{}, T{}); }
    static TVec3 One() { return TVec3(T(1), T(1), T(1)); }

    static TVec3 Left() { return TVec3(T(1), T(0), T(0)); }
    static TVec3 Up() { return TVec3(T(0), T(1), T(0)); }
    static TVec3 Forward() { return TVec3(T(0), T(0), T(1)); }

    TVec3 operator-() const
    {
        return TVec3(-x, -y, -z);
    }

    TVec3& operator+=(const TVec3& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    TVec3 operator+(const TVec3& rhs) const
    {
        return TVec3(*this) += rhs;
    }

    TVec3& operator-=(const TVec3& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    TVec3 operator-(const TVec3& rhs) const
    {
        return TVec3(*this) -= rhs;
    }

    TVec3& operator*=(const T& rhs)
    {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        return *this;
    }

    TVec3 operator*(const T& rhs) const
    {
        return TVec3(*this) *= rhs;
    }

    TVec3& operator/=(const T& rhs)
    {
        x /= rhs;
        y /= rhs;
        z /= rhs;
        return *this;
    }

    TVec3 operator/(const T& rhs) const
    {
        return TVec3(*this) /= rhs;
    }

    bool operator==(const TVec3& rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z; }
    bool operator!=(const TVec3& rhs) const { return !(*this == rhs); }

    T LengthSqr() const { return x * x + y * y + z * z; }
    T Length() const { return Sqrt(LengthSqr()); }

    TVec3& Normalized()
    {
        const T len_inv = T(1) / Length();
        x *= len_inv;
        y *= len_inv;
        z *= len_inv;
        return *this;
    }

    TVec3 Normalize() const
    {
        TVec3 norm = *this;
        norm.Normalized();
        return norm;
    }

    TVec3 Cross(const TVec3& rhs) const
    {
        return TVec3(
            y * rhs.z - z * rhs.y,
            z * rhs.x - x * rhs.z,
            x * rhs.y - y * rhs.x
        );
    }

    T Dot(const TVec3& rhs) const
    {
        return x * rhs.x + y * rhs.y + z * rhs.z;
    }
};

using Vec3 = TVec3<float>;
using Vec3d = TVec3<double>;
using Vec3i = TVec3<int32_t>;

template <typename T>
struct TVec4
{
    T x = {};
    T y = {};
    T z = {};
    T w = {};

    TVec4() = default;
    TVec4(const TVec4&) = default;
    TVec4& operator=(const TVec4&) = default;
    TVec4(TVec4&&) = default;
    TVec4& operator=(TVec4&&) = default;

    TVec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}

    template <typename TOther>
    explicit operator TVec4<TOther>() const
    {
        return TVec4<TOther>(static_cast<TOther>(x), static_cast<TOther>(y), static_cast<TOther>(z), static_cast<TOther>(w));
    }

    static TVec4 Zero() { return TVec4(T{}, T{}, T{}, T{}); }
    static TVec4 One() { return TVec4(T(1), T(1), T(1), T(1)); }

    TVec4 operator-() const
    {
        return TVec4(-x, -y, -z, -w);
    }

    TVec4& operator+=(const TVec4& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        w += rhs.w;
        return *this;
    }

    TVec4 operator+(const TVec4& rhs) const
    {
        return TVec4(*this) += rhs;
    }

    TVec4& operator-=(const TVec4& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        w -= rhs.w;
        return *this;
    }

    TVec4 operator-(const TVec4& rhs) const
    {
        return TVec4(*this) -= rhs;
    }

    TVec4& operator*=(const T& rhs)
    {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        w *= rhs;
        return *this;
    }

    TVec4 operator*(const T& rhs) const
    {
        return TVec4(*this) *= rhs;
    }

    TVec4& operator/=(const T& rhs)
    {
        x /= rhs;
        y /= rhs;
        z /= rhs;
        w /= rhs;
        return *this;
    }

    TVec4 operator/(const T& rhs) const
    {
        return TVec4(*this) /= rhs;
    }

    bool operator==(const TVec4& rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w; }
    bool operator!=(const TVec4& rhs) const { return !(*this == rhs); }

    T LengthSqr() const { return x * x + y * y + z * z + w * w; }
    T Length() const { return Sqrt(LengthSqr()); }

    TVec4& Normalized()
    {
        const T len_inv = T(1) / Length();
        x *= len_inv;
        y *= len_inv;
        z *= len_inv;
        w *= len_inv;
        return *this;
    }

    TVec4 Normalize() const
    {
        TVec4 norm = *this;
        norm.Normalized();
        return norm;
    }

    T Dot(const TVec4& rhs) const
    {
        return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
    }
};

using Vec4 = TVec4<float>;
using Vec4d = TVec4<double>;
using Vec4i = TVec4<int32_t>;

template <typename T>
struct TMat2
{
    TVec2<T> col0;
    TVec2<T> col1;

    TMat2() = default;
    TMat2(const TMat2&) = default;
    TMat2& operator=(const TMat2&) = default;
    TMat2(TMat2&&) = default;
    TMat2& operator=(TMat2&&) = default;

    TMat2(const TVec2<T>& col0, const TVec2<T>& col1) : col0(col0), col1(col1) {}
    TMat2(T a00, T a01, T a10, T a11)
        : col0(a00, a10)
        , col1(a01, a11)
    {
    }

    template <typename TOther>
    explicit operator TMat2<TOther>() const
    {
        return TMat2<TOther>(
            static_cast<TOther>(col0.x),
            static_cast<TOther>(col1.x),
            static_cast<TOther>(col0.y),
            static_cast<TOther>(col1.y)
        );
    }

    TVec2<T> Row0() const
    {
        return TVec2<T>(col0.x, col1.x);
    }
    TVec2<T> Row1() const
    {
        return TVec2<T>(col0.y, col1.y);
    }

    static TMat2 Identity()
    {
        return TMat2(
            TVec2<T>(1, 0),
            TVec2<T>(0, 1)
        );
    }
    static TMat2 Zero()
    {
        return TMat2(
            TVec2<T>(0, 0),
            TVec2<T>(0, 0)
        );
    }

    TMat2 operator-() const
    {
        return TMat2(-col0, -col1);
    }

    TMat2& operator+=(const TMat2& rhs)
    {
        col0 += rhs.col0;
        col1 += rhs.col1;
        return *this;
    }
    TMat2 operator+(const TMat2& rhs) const
    {
        return TMat2(*this) += rhs;
    }
    TMat2& operator-=(const TMat2& rhs)
    {
        col0 -= rhs.col0;
        col1 -= rhs.col1;
        return *this;
    }
    TMat2 operator-(const TMat2& rhs) const
    {
        return TMat2(*this) -= rhs;
    }

    TMat2& operator*=(const T& rhs)
    {
        col0 *= rhs;
        col1 *= rhs;
        return *this;
    }
    TMat2 operator*(const T& rhs) const
    {
        return TMat2(*this) *= rhs;
    }
    TMat2& operator/=(const T& rhs)
    {
        col0 /= rhs;
        col1 /= rhs;
        return *this;
    }
    TMat2 operator/(const T& rhs) const
    {
        return TMat2(*this) /= rhs;
    }

    TVec2<T> operator*(const TVec2<T>& rhs) const
    {
        return TVec2<T>(Row0().Dot(rhs), Row1().Dot(rhs));
    }

    TMat2& operator*=(const TMat2& rhs)
    {
        TVec2<T> row0 = Row0();
        TVec2<T> row1 = Row1();

        col0 = TVec2(row0.Dot(rhs.col0), row1.Dot(rhs.col0));
        col1 = TVec2(row0.Dot(rhs.col1), row1.Dot(rhs.col1));

        return *this;
    }
    TMat2 operator*(const TMat2& rhs) const
    {
        return TMat2(*this) *= rhs;
    }

    bool operator==(const TMat2& rhs) const { return col0 == rhs.col0 && col1 == rhs.col1; }
    bool operator!=(const TMat2& rhs) const { return !(*this == rhs); }

    T Determinant() const
    {
        return col0.x * col1.y - col0.y * col1.x;
    }

    T Trace() const
    {
        return col0.x + col1.y;
    }

    TMat2& Transposed()
    {
        TVec2<T> new_col0(col0.x, col1.x);
        TVec2<T> new_col1(col0.y, col1.y);
        col0 = new_col0;
        col1 = new_col1;

        return *this;
    }
    TMat2 Transpose() const
    {
        return TMat2(*this).Transposed();
    }
};

using Mat2 = TMat2<float>;
using Mat2d = TMat2<double>;
using Mat2i = TMat2<int32_t>;

template <typename T>
struct TMat3
{
    TVec3<T> col0;
    TVec3<T> col1;
    TVec3<T> col2;

    TMat3() = default;
    TMat3(const TMat3&) = default;
    TMat3& operator=(const TMat3&) = default;
    TMat3(TMat3&&) = default;
    TMat3& operator=(TMat3&&) = default;

    TMat3(const TVec3<T>& col0, const TVec3<T>& col1, const TVec3<T>& col2) : col0(col0), col1(col1), col2(col2) {}
    TMat3(T a00, T a01, T a02, T a10, T a11, T a12, T a20, T a21, T a22)
        : col0(a00, a10, a10)
        , col1(a01, a11, a11)
        , col2(a02, a12, a12)
    {
    }

    template <typename TOther>
    explicit operator TMat3<TOther>() const
    {
        return TMat3<TOther>(
            static_cast<TOther>(col0.x),
            static_cast<TOther>(col1.x),
            static_cast<TOther>(col2.x),
            static_cast<TOther>(col0.y),
            static_cast<TOther>(col1.y),
            static_cast<TOther>(col2.y),
            static_cast<TOther>(col0.z),
            static_cast<TOther>(col1.z),
            static_cast<TOther>(col2.z)
        );
    }

    TVec3<T> Row0() const
    {
        return TVec3<T>(col0.x, col1.x, col2.x);
    }
    TVec3<T> Row1() const
    {
        return TVec3<T>(col0.y, col1.y, col2.y);
    }
    TVec3<T> Row2() const
    {
        return TVec3<T>(col0.z, col1.z, col2.z);
    }

    static TMat3 Identity()
    {
        return TMat3(
            TVec3<T>(1, 0, 0),
            TVec3<T>(0, 1, 0),
            TVec3<T>(0, 0, 1)
        );
    }
    static TMat3 Zero()
    {
        return TMat3(
            TVec3<T>(0, 0, 0),
            TVec3<T>(0, 0, 0),
            TVec3<T>(0, 0, 0)
        );
    }

    TMat3 operator-() const
    {
        return TMat3(-col0, -col1, -col2);
    }

    TMat3& operator+=(const TMat3& rhs)
    {
        col0 += rhs.col0;
        col1 += rhs.col1;
        col2 += rhs.col2;
        return *this;
    }
    TMat3 operator+(const TMat3& rhs) const
    {
        return TMat3(*this) += rhs;
    }
    TMat3& operator-=(const TMat3& rhs)
    {
        col0 -= rhs.col0;
        col1 -= rhs.col1;
        col2 -= rhs.col2;
        return *this;
    }
    TMat3 operator-(const TMat3& rhs) const
    {
        return TMat3(*this) -= rhs;
    }

    TMat3& operator*=(const T& rhs)
    {
        col0 *= rhs;
        col1 *= rhs;
        col2 *= rhs;
        return *this;
    }
    TMat3 operator*(const T& rhs) const
    {
        return TMat3(*this) *= rhs;
    }
    TMat3& operator/=(const T& rhs)
    {
        col0 /= rhs;
        col1 /= rhs;
        col2 /= rhs;
        return *this;
    }
    TMat3 operator/(const T& rhs) const
    {
        return TMat3(*this) /= rhs;
    }

    TVec3<T> operator*(const TVec3<T>& rhs) const
    {
        return TVec3<T>(Row0().Dot(rhs), Row1().Dot(rhs), Row2().Dot(rhs));
    }

    TMat3& operator*=(const TMat3& rhs)
    {
        TVec3<T> row0 = Row0();
        TVec3<T> row1 = Row1();
        TVec3<T> row2 = Row2();

        col0 = TVec3(row0.Dot(rhs.col0), row1.Dot(rhs.col0), row2.Dot(rhs.col0));
        col1 = TVec3(row0.Dot(rhs.col1), row1.Dot(rhs.col1), row2.Dot(rhs.col1));
        col2 = TVec3(row0.Dot(rhs.col2), row1.Dot(rhs.col2), row2.Dot(rhs.col2));

        return *this;
    }
    TMat3 operator*(const TMat3& rhs) const
    {
        return TMat3(*this) *= rhs;
    }

    bool operator==(const TMat3& rhs) const { return col0 == rhs.col0 && col1 == rhs.col1 && col2 == rhs.col2; }
    bool operator!=(const TMat3& rhs) const { return !(*this == rhs); }

    T Determinant() const
    {
        return col0.x * (col1.y * col2.z - col1.z * col2.y) -
            col0.y * (col1.x * col2.z - col1.z * col2.x) +
            col0.z * (col1.x * col2.y - col1.y * col2.x);
    }

    T Trace() const
    {
        return col0.x + col1.y + col2.z;
    }

    TMat3& Transposed()
    {
        TVec3<T> new_col0(col0.x, col1.x, col2.x);
        TVec3<T> new_col1(col0.y, col1.y, col2.y);
        TVec3<T> new_col2(col0.z, col1.z, col2.z);
        col0 = new_col0;
        col1 = new_col1;
        col2 = new_col2;
        return *this;
    }
    TMat3 Transpose() const
    {
        return TMat3(*this).Transposed();
    }
};

using Mat3 = TMat3<float>;
using Mat3d = TMat3<double>;
using Mat3i = TMat3<int32_t>;

template <typename T>
struct TMat4
{
    TVec4<T> col0;
    TVec4<T> col1;
    TVec4<T> col2;
    TVec4<T> col3;

    TMat4() = default;
    TMat4(const TMat4&) = default;
    TMat4& operator=(const TMat4&) = default;
    TMat4(TMat4&&) = default;
    TMat4& operator=(TMat4&&) = default;

    TMat4(const TVec4<T>& col0, const TVec4<T>& col1, const TVec4<T>& col2, const TVec4<T>& col3)
        : col0(col0)
        , col1(col1)
        , col2(col2)
        , col3(col3)
    {}
    TMat4(T a00, T a01, T a02, T a03, T a10, T a11, T a12, T a13, T a20, T a21, T a22, T a23, T a30, T a31, T a32, T a33)
        : col0(a00, a10, a10, a20)
        , col1(a01, a11, a11, a21)
        , col2(a02, a12, a12, a22)
        , col3(a03, a13, a13, a23)
    {
    }

    template <typename TOther>
    explicit operator TMat4<TOther>() const
    {
        return TMat4<TOther>(
            static_cast<TOther>(col0.x),
            static_cast<TOther>(col1.x),
            static_cast<TOther>(col2.x),
            static_cast<TOther>(col3.x),
            static_cast<TOther>(col0.y),
            static_cast<TOther>(col1.y),
            static_cast<TOther>(col2.y),
            static_cast<TOther>(col3.y),
            static_cast<TOther>(col0.z),
            static_cast<TOther>(col1.z),
            static_cast<TOther>(col2.z),
            static_cast<TOther>(col3.z),
            static_cast<TOther>(col0.w),
            static_cast<TOther>(col1.w),
            static_cast<TOther>(col2.w),
            static_cast<TOther>(col3.w)
        );
    }

    TVec4<T> Row0() const
    {
        return TVec4<T>(col0.x, col1.x, col2.x, col3.x);
    }
    TVec4<T> Row1() const
    {
        return TVec4<T>(col0.y, col1.y, col2.y, col3.y);
    }
    TVec4<T> Row2() const
    {
        return TVec4<T>(col0.z, col1.z, col2.z, col3.z);
    }
    TVec4<T> Row3() const
    {
        return TVec4<T>(col0.w, col1.w, col2.w, col3.w);
    }

    static TMat4 Identity()
    {
        return TMat4(
            TVec4<T>(1, 0, 0, 0),
            TVec4<T>(0, 1, 0, 0),
            TVec4<T>(0, 0, 1, 0),
            TVec4<T>(0, 0, 0, 1)
        );
    }
    static TMat4 Zero()
    {
        return TMat4(
            TVec4<T>(0, 0, 0, 0),
            TVec4<T>(0, 0, 0, 0),
            TVec4<T>(0, 0, 0, 0),
            TVec4<T>(0, 0, 0, 0)
        );
    }

    TMat4 operator-() const
    {
        return TMat4(-col0, -col1, -col2, -col3);
    }

    TMat4& operator+=(const TMat4& rhs)
    {
        col0 += rhs.col0;
        col1 += rhs.col1;
        col2 += rhs.col2;
        return *this;
    }
    TMat4 operator+(const TMat4& rhs) const
    {
        return TMat4(*this) += rhs;
    }
    TMat4& operator-=(const TMat4& rhs)
    {
        col0 -= rhs.col0;
        col1 -= rhs.col1;
        col2 -= rhs.col2;
        return *this;
    }
    TMat4 operator-(const TMat4& rhs) const
    {
        return TMat4(*this) -= rhs;
    }

    TMat4& operator*=(const T& rhs)
    {
        col0 *= rhs;
        col1 *= rhs;
        col2 *= rhs;
        return *this;
    }
    TMat4 operator*(const T& rhs) const
    {
        return TMat4(*this) *= rhs;
    }
    TMat4& operator/=(const T& rhs)
    {
        col0 /= rhs;
        col1 /= rhs;
        col2 /= rhs;
        return *this;
    }
    TMat4 operator/(const T& rhs) const
    {
        return TMat4(*this) /= rhs;
    }

    TVec4<T> operator*(const TVec4<T>& rhs) const
    {
        return TVec4<T>(Row0().Dot(rhs), Row1().Dot(rhs), Row2().Dot(rhs), Row3().Dot(rhs));
    }

    TMat4& operator*=(const TMat4& rhs)
    {
        TVec4<T> row0 = Row0();
        TVec4<T> row1 = Row1();
        TVec4<T> row2 = Row2();
        TVec4<T> row3 = Row3();

        col0 = TVec3(row0.Dot(rhs.col0), row1.Dot(rhs.col0), row2.Dot(rhs.col0), row3.Dot(rhs.col0));
        col1 = TVec3(row0.Dot(rhs.col1), row1.Dot(rhs.col1), row2.Dot(rhs.col1), row3.Dot(rhs.col1));
        col2 = TVec3(row0.Dot(rhs.col2), row1.Dot(rhs.col2), row2.Dot(rhs.col2), row3.Dot(rhs.col2));
        col3 = TVec3(row0.Dot(rhs.col3), row1.Dot(rhs.col3), row2.Dot(rhs.col3), row3.Dot(rhs.col3));

        return *this;
    }
    TMat4 operator*(const TMat4& rhs) const
    {
        return TMat4(*this) *= rhs;
    }

    bool operator==(const TMat4& rhs) const { return col0 == rhs.col0 && col1 == rhs.col1 && col2 == rhs.col2 && col3 == rhs.col3; }
    bool operator!=(const TMat4& rhs) const { return !(*this == rhs); }

    T Determinant() const
    {
        return col0.x * (col1.y * (col2.z * col3.w - col2.w * col3.z) - col1.z * (col2.y * col3.w - col2.w * col3.y) + col1.w * (col2.y * col3.z - col2.z * col3.y)) -
            col0.y * (col1.x * (col2.z * col3.w - col2.w * col3.z) - col1.z * (col2.x * col3.w - col2.w * col3.x) + col1.w * (col2.x * col3.z - col2.z * col3.x)) +
            col0.z * (col1.x * (col2.y * col3.w - col2.w * col3.y) - col1.y * (col2.x * col3.w - col2.w * col3.x) + col1.w * (col2.x * col3.y - col2.y * col3.x)) -
            col0.w * (col1.x * (col2.y * col3.z - col2.z * col3.y) - col1.y * (col2.x * col3.z - col2.z * col3.x) + col1.z * (col2.x * col3.y - col2.y * col3.x));
    }

    T Trace() const
    {
        return col0.x + col1.y + col2.z + col3.w;
    }

    TMat4& Transposed()
    {
        TVec4<T> new_col0(col0.x, col1.x, col2.x, col3.x);
        TVec4<T> new_col1(col0.y, col1.y, col2.y, col3.y);
        TVec4<T> new_col2(col0.z, col1.z, col2.z, col3.z);
        TVec4<T> new_col3(col0.w, col1.w, col2.w, col3.w);
        col0 = new_col0;
        col1 = new_col1;
        col2 = new_col2;
        col3 = new_col3;
        return *this;
    }
    TMat4 Transpose() const
    {
        return TMat4(*this).Transposed();
    }
};

using Mat4 = TMat4<float>;
using Mat4d = TMat4<double>;
using Mat4i = TMat4<int32_t>;
