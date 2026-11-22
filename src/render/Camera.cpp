#include "Camera.hpp"

#include <algorithm>
#include <cmath>

Camera::Camera()
    : position_(0.0f, 0.0f, 5.0f),
      rotation_(0.0f, 0.0f, 0.0f)
{
}

const Vec3& Camera::position() const {
    return position_;
}

const Vec3& Camera::rotation() const {
    return rotation_;
}

void Camera::setPosition(const Vec3& position) {
    position_ = position;
}

void Camera::setRotation(const Vec3& rotation) {
    rotation_ = rotation;

    // Pitch: не даём камере перевернуться
    float pitch = std::clamp(rotation_.x(), -89.0f, 89.0f);

    rotation_ = Vec3(
        pitch,
        rotation_.y(),
        0.0f
    );
}

void Camera::move(const Vec3& offset) {
    position_ = position_ + offset;
}

void Camera::rotate(const Vec3& offset) {
    rotation_ = Vec3(
        rotation_.x() + offset.x(),
        rotation_.y() + offset.y(),
        0.0f
    );

    float pitch = std::clamp(
        rotation_.x(),
        -89.0f,
        89.0f
    );

    rotation_ = Vec3(
        pitch,
        rotation_.y(),
        0.0f
    );
}

Vec3 Camera::forward() const
{
    float yaw = rotation_.y() * (M_PI / 180.0f);
    float pitch = rotation_.x() * (M_PI / 180.0f);

    return Vec3(
        std::sin(yaw) * std::cos(pitch),
        -std::sin(pitch),
        -std::cos(yaw) * std::cos(pitch)
    ).normalized();
}

Vec3 Camera::right() const
{
    Vec3 forward = this->forward();

    return Vec3(
        -forward.z(),
        0.0f,
        forward.x()
    ).normalized();
}
Matrix4 Camera::viewMatrix() const
{
    Matrix4 translation = Matrix4::translation(
        -position_.x(),
        -position_.y(),
        -position_.z()
    );

    Matrix4 rotationY = Matrix4::rotationY(
        rotation_.y()
    );

    Matrix4 rotationX = Matrix4::rotationX(
        -rotation_.x()
    );

    return rotationY * rotationX * translation;
}