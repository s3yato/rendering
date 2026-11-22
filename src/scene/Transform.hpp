#pragma once

#include "Vec3.hpp"
#include "Matrix4.hpp"

class Transform {
public:
    Transform();

    void setPosition(const Vec3& p);
    void setScale(const Vec3& s);
    void setRotation(const Vec3& r);

    const Vec3& position() const;
    const Vec3& scale() const;
    const Vec3& rotation() const;

    Matrix4 modelMatrix() const;

private:
    Vec3 position_;
    Vec3 scale_;
    Vec3 rotation_;
};