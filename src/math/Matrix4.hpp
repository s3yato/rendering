#pragma once

#include <array>
#include <cstddef>
#include <cmath>

#include "Vec4.hpp"

class Matrix4 {
    public:
        Matrix4();

        float& at(std::size_t row, std::size_t col);
        float at(std::size_t row, std::size_t col) const;

        Vec4 operator*(const Vec4& vector) const;
        Matrix4 operator*(const Matrix4& other) const;

        static Matrix4 identity();

        static Matrix4 scale(float x, float y, float z);

        static Matrix4 translation(float tx, float ty, float tz);
        static Matrix4 rotationZ(float angle);
        static Matrix4 rotationX(float angle);
        static Matrix4 rotationY(float angle);
        static Matrix4 perspective(
            float fov,
            float aspect,
            float near,
            float far
        );

    private:
        std::array<float, 16> data_;
};