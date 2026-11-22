#pragma once

#include <cmath>

class Vec2
{
public:
    Vec2(float x = 0.0f,
         float y = 0.0f)
        : x_(x), y_(y)
    {
    }

    Vec2 operator+(const Vec2& other) const
    {
        return Vec2(
            x_ + other.x_,
            y_ + other.y_
        );
    }

    Vec2 operator-(const Vec2& other) const
    {
        return Vec2(
            x_ - other.x_,
            y_ - other.y_
        );
    }

    Vec2 operator*(float value) const
    {
        return Vec2(
            x_ * value,
            y_ * value
        );
    }

    Vec2 operator/(float value) const
    {
        return Vec2(
            x_ / value,
            y_ / value
        );
    }

    float length() const
    {
        return std::sqrt(x_ * x_ + y_ * y_);
    }

    Vec2 normalized() const
    {
        float l = length();

        if (l < 1e-6f)
            return Vec2();

        return *this / l;
    }

    float dot(const Vec2& other) const
    {
        return x_ * other.x_
             + y_ * other.y_;
    }

    float x() const { return x_; }
    float y() const { return y_; }

private:
    float x_;
    float y_;
};