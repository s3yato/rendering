#include "Light.hpp"

Light::Light(const Vec3& direction): direction_(direction.normalized()) {}

const Vec3& Light::direction() const {
    return direction_;
}
