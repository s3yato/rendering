#include "Matrix4.hpp"

Matrix4::Matrix4()
{
    data_.fill(0.0f);
}

float& Matrix4::at(std::size_t row, std::size_t col) {
    return data_[row * 4 + col];
}

float Matrix4::at(std::size_t row, std::size_t col) const {
    return data_[row * 4 + col];
}

Vec4 Matrix4::operator*(const Vec4& vector) const {
    float x = 
        at(0, 0) * vector.x() +
        at(0, 1) * vector.y() + 
        at(0, 2) * vector.z() + 
        at(0, 3) * vector.w();
    float y = 
        at(1, 0) * vector.x() +
        at(1, 1) * vector.y() + 
        at(1, 2) * vector.z() + 
        at(1, 3) * vector.w();
    float z = 
        at(2, 0) * vector.x() +
        at(2, 1) * vector.y() + 
        at(2, 2) * vector.z() + 
        at(2, 3) * vector.w();
    float w = 
        at(3, 0) * vector.x() +
        at(3, 1) * vector.y() + 
        at(3, 2) * vector.z() + 
        at(3, 3) * vector.w();

    return Vec4(x, y, z, w);
}
Matrix4 Matrix4::operator*(const Matrix4& other) const {
    Matrix4 matrix;

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            matrix.at(row, col) = 0; 

            for (int k = 0; k < 4; k++) {
                matrix.at(row, col) += at(row, k) * other.at(k, col);
            }
        }
    }

    return matrix;
}
Matrix4 Matrix4::identity() {
    Matrix4 matrix;

    matrix.at(0, 0) = 1.0f;
    matrix.at(1, 1) = 1.0f;
    matrix.at(2, 2) = 1.0f;
    matrix.at(3, 3) = 1.0f;

    return matrix;
}

Matrix4 Matrix4::scale(float x, float y, float z) {
    Matrix4 matrix = Matrix4::identity();

    matrix.at(0,0) = x;
    matrix.at(1,1) = y;
    matrix.at(2,2) = z;

    return matrix;
}

Matrix4 Matrix4::translation(float tx, float ty, float tz) {
    Matrix4 matrix = identity();

    matrix.at(0,3) = tx;
    matrix.at(1,3) = ty;
    matrix.at(2,3) = tz;

    return matrix;
}

Matrix4 Matrix4::rotationZ(float angle) {
    float a = angle * (M_PI / 180.0);

    Matrix4 matrix = identity();

    matrix.at(0, 0) = std::cos(a);
    matrix.at(0, 1) = std::sin(a) * -1;
    matrix.at(1, 0) = std::sin(a);
    matrix.at(1, 1) = std::cos(a);

    return matrix;
}

Matrix4 Matrix4::rotationX(float angle) {
    float a = angle * (M_PI / 180.0);

    Matrix4 matrix = identity();

    matrix.at(1, 1) = std::cos(a);
    matrix.at(1, 2) = std::sin(a) * -1;
    matrix.at(2, 1) = std::sin(a);
    matrix.at(2, 2) = std::cos(a);

    return matrix;
}

Matrix4 Matrix4::rotationY(float angle) {
    float a = angle * (M_PI / 180.0);

    Matrix4 matrix = identity();

    matrix.at(0, 0) = std::cos(a);
    matrix.at(0, 2) = std::sin(a);
    matrix.at(2, 0) = std::sin(a) * -1;
    matrix.at(2, 2) = std::cos(a);

    return matrix;
}
Matrix4 Matrix4::perspective(
    float fov,
    float aspect,
    float nearPlane,
    float farPlane
) {
    Matrix4 matrix;

    float radians = fov * (M_PI / 180.0f);
    float f = 1.0f / std::tan(radians / 2.0f);

    matrix.at(0, 0) = f / aspect;
    matrix.at(1, 1) = f;

    matrix.at(2, 2) =
        (farPlane + nearPlane) / (nearPlane - farPlane);

    matrix.at(2, 3) =
        (2.0f * farPlane * nearPlane) /
        (nearPlane - farPlane);

    matrix.at(3, 2) = -1.0f;

    return matrix;
}