#pragma once

#include <algorithm>
#include <cmath>

// simple vector for some basic vector arithmetic
struct Vec3
{
    float x, y, z;

    Vec3()
        : x(0.f)
        , y(0.f)
        , z(0.f)
    {}

    Vec3(float xVal, float yVal, float zVal)
        : x(xVal)
        , y(yVal)
        , z(zVal)
    {}

    Vec3 operator+(const Vec3& other) const noexcept
    {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }

    Vec3 operator-(const Vec3& other) const noexcept
    {
        return Vec3(x - other.x, y - other.y, z - other.z);
    }

    Vec3 operator*(const Vec3& other)
    {
        return Vec3(x * other.x, y * other.y, z * other.z);
    }

    Vec3 operator*(float scale) const noexcept
    {
        return Vec3(x * scale, y * scale, z * scale);
    }

    Vec3 operator/(const Vec3& other) const noexcept
    {
        return Vec3(x / other.x, y / other.y, z / other.z);
    }

    Vec3 operator/(float invScale) const noexcept
    {
        return Vec3(x / invScale, y / invScale, z / invScale);
    }

    static float Dot(const Vec3& a, const Vec3& b) noexcept
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    static float Length(const Vec3& vec) noexcept
    {
        return std::sqrtf(Dot(vec, vec));
    }

    static Vec3 Cross(const Vec3& a, const Vec3& b) noexcept
    {
        return Vec3(
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        );
    }

    static Vec3 Min(const Vec3& a, const Vec3& b) noexcept
    {
        return Vec3(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
    }

    static Vec3 Max(const Vec3& a, const Vec3& b) noexcept
    {
        return Vec3(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
    }

    static Vec3 Normalize(const Vec3& vec) noexcept
    {
        return vec / Length(vec);
    }
};