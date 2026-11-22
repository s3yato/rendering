
#pragma once

#include "Vec3.hpp"

class Triangle
{
public:
    Triangle(const Vec3& a,
             const Vec3& b,
             const Vec3& c);

    const Vec3& a() const;
    const Vec3& b() const;
    const Vec3& c() const;

    Vec3 normal() const;

private:
    Vec3 a_;
    Vec3 b_;
    Vec3 c_;
};