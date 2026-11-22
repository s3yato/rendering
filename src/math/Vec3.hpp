#pragma once

#include <cmath>

class Vec3
{
public:
    Vec3(float x = 0.0f,
         float y = 0.0f,
         float z = 0.0f)
        : x_(x), y_(y), z_(z)
    {
    }

    Vec3 operator+(const Vec3& other) const
    {
        return Vec3(
            x_ + other.x_,
            y_ + other.y_,
            z_ + other.z_
        );
    }

    Vec3 operator-(const Vec3& other) const
    {
        return Vec3(
            x_ - other.x_,
            y_ - other.y_,
            z_ - other.z_
        );
    }

    Vec3 operator*(float value) const
    {
        return Vec3(
            x_ * value,
            y_ * value,
            z_ * value
        );
    }

    Vec3 operator/(float value) const
    {
        return Vec3(
            x_ / value,
            y_ / value,
            z_ / value
        );
    }

    float length() const
    {
        return std::sqrt(x_ * x_ + y_ * y_ + z_ * z_);
    }

    Vec3 normalized() const
    {
        float l = length();

        if (l < 1e-6f)
            return Vec3();

        return *this / l;
    }

    float dot(const Vec3& other) const
    {
        return x_ * other.x_
             + y_ * other.y_
             + z_ * other.z_;
    }

    Vec3 cross(const Vec3& other) const
    {
        return Vec3(
            y_ * other.z_ - z_ * other.y_,
            z_ * other.x_ - x_ * other.z_,
            x_ * other.y_ - y_ * other.x_
        );
    }

    float x() const { return x_; }
    float y() const { return y_; }
    float z() const { return z_; }

private:
    float x_;
    float y_;
    float z_;
};