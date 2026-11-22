#pragma once

#include "Vec3.hpp"
#include "Matrix4.hpp"

class Camera {
public:
    Camera();

    const Vec3& position() const;
    const Vec3& rotation() const;

    void setPosition(const Vec3& position);
    void setRotation(const Vec3& rotation);

    void move(const Vec3& offset);
    void rotate(const Vec3& offset);

    Vec3 forward() const;
    Vec3 right() const;

    Matrix4 viewMatrix() const;

private:
    Vec3 position_;
    Vec3 rotation_;
};