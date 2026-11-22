#include "Triangle.hpp"

Triangle::Triangle(const Vec3& a,
                const Vec3& b,
                const Vec3& c)
    : a_(a),
        b_(b),
        c_(c)
{
}

const Vec3& Triangle::a() const
{
    return a_;
}

const Vec3& Triangle::b() const
{
    return b_;
}

const Vec3& Triangle::c() const
{
    return c_;
}

Vec3 Triangle::normal() const
{
    Vec3 ab = b_ - a_;
    Vec3 ac = c_ - a_;

    return ab.cross(ac).normalized();
}