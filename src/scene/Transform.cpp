#include "Transform.hpp"

Transform::Transform()
    : position_(0, 0, 0),
      rotation_(0, 0, 0),
      scale_(1, 1, 1)
{
}

void Transform::setPosition(const Vec3& p) {
    position_ = p;
}

void Transform::setScale(const Vec3& s) {
    scale_ = s;
}

void Transform::setRotation(const Vec3& r) {
    rotation_ = r;
}

const Vec3& Transform::position() const {
    return position_;
}

const Vec3& Transform::scale() const {
    return scale_;
}

const Vec3& Transform::rotation() const {
    return rotation_;
}

Matrix4 Transform::modelMatrix() const {
    Matrix4 s = Matrix4::scale(
        scale_.x(),
        scale_.y(),
        scale_.z()
    );

    Matrix4 rx = Matrix4::rotationX(rotation_.x());
    Matrix4 ry = Matrix4::rotationY(rotation_.y());
    Matrix4 rz = Matrix4::rotationZ(rotation_.z());

    Matrix4 t = Matrix4::translation(
        position_.x(),
        position_.y(),
        position_.z()
    );

    return t * rz * ry * rx * s;
}