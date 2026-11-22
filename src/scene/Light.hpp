#pragma once

#include "Vec3.hpp"

class Light {
public:
    explicit Light(const Vec3& direction_);

    const Vec3& direction() const;

private:
    Vec3 direction_;
};