#include "AobaAPI/Math/Matrix/Matrix3.hpp"

#include <algorithm>
#include <array>

namespace Aoba {
namespace Math {

Mat3::Mat3() {
    for(int i = 0; i < 9; i++) {
        data[i] = 0;
    }
}

Mat3::Mat3(const std::array<float, 9>& vals) {
    for(int i = 0; i < 9; i++) {
        data[i] = vals.at(i);
    }
}

Mat3 Mat3::Diagonal(const Vec3& vec) {
    Mat3 result = Mat3();
    result.data[0] = vec.x;
    result.data[4] = vec.y;
    result.data[8] = vec.z;
    return result;
}

Mat3 Mat3::Identity() {
    Mat3 result = Mat3();
    result.data[0] = 1;
    result.data[4] = 1;
    result.data[8] = 1;
    return result;
}

Mat3 Mat3::OrthoProjection(const Vec3& axis) {
    Mat3 result = Mat3();
    Vec3 axisNorm = axis.Normalized();
    result.data[0] = 1 - powf(axisNorm.x, 2);
    result.data[1] = -axisNorm.x * axisNorm.y;
    result.data[2] = -axisNorm.x * axisNorm.z;
    result.data[3] = -axisNorm.y * axisNorm.x;
    result.data[4] = 1 - powf(axisNorm.y, 2);
    result.data[5] = -axisNorm.y * axisNorm.z;
    result.data[6] = -axisNorm.z * axisNorm.x;
    result.data[7] = -axisNorm.z * axisNorm.y;
    result.data[8] = 1 - powf(axisNorm.z, 2);
    return result;
}

Mat3 Mat3::Rotation(const Vec3& axis, float angle) {
    float angleCos = cosf(angle);
    float angleSin = sinf(angle);
    Mat3 result = Mat3();
    result.data[0] = axis.x * axis.x * (1 - angleCos) + angleCos;
    result.data[1] = axis.x * axis.y * (1 - angleCos) - axis.z * angleSin;
    result.data[2] = axis.x * axis.z * (1 - angleCos) + axis.y * angleSin;
    result.data[3] = axis.x * axis.y * (1 - angleCos) + axis.z * angleSin;
    result.data[4] = axis.y * axis.y * (1 - angleCos) + angleCos;
    result.data[5] = axis.y * axis.z * (1 - angleCos) - axis.x * angleSin;
    result.data[6] = axis.x * axis.z * (1 - angleCos) - axis.y * angleSin;
    result.data[7] = axis.y * axis.z * (1 - angleCos) + axis.x * angleSin;
    result.data[8] = axis.z * axis.z * (1 - angleCos) + angleCos;
    return result;
}

Mat3 Mat3::Scale(const Vec3& axis, float factor) {
    // taken from
    // https://www.mauriciopoppe.com/notes/computer-graphics/transformation-matrices/scale/#scaling-along-an-arbitrary-axis
    Vec3 axisNorm = axis.Normalized();
    Mat3 result = Mat3();
    float fac = factor - 1;
    result.data[0] = 1 + fac * powf(axisNorm.x, 2);
    result.data[1] = fac * axisNorm.x * axisNorm.y;
    result.data[2] = fac * axisNorm.x * axisNorm.z;
    result.data[3] = fac * axisNorm.x * axisNorm.y;
    result.data[4] = 1 + fac * powf(axisNorm.y, 2);
    result.data[5] = fac * axisNorm.y * axisNorm.z;
    result.data[6] = fac * axisNorm.x * axisNorm.z;
    result.data[7] = fac * axisNorm.y * axisNorm.z;
    result.data[8] = 1 + fac * powf(axisNorm.z, 2);
    return result;
}

Mat3 Mat3::Zero() {
    return Mat3();
}

float Mat3::Determinant() const {
    float result = 0;
    result += data[0] * data[4] * data[8];
    result += data[1] * data[5] * data[6];
    result += data[2] * data[3] * data[7];
    result -= data[2] * data[4] * data[6];
    result -= data[1] * data[3] * data[8];
    result -= data[0] * data[5] * data[7];
    return result;
}

bool Mat3::Equals(const Mat3& other, float epsilon) {
    for(std::size_t idx = 0; idx < 9; idx++) {
        if(abs(data[idx] - other.data[idx]) < epsilon) {
            return false;
        }
    }
    return true;
}

Vec3 Mat3::GetScale() const {
    Vec3 vecx = Vec3(data[0], data[3], data[6]);
    Vec3 vecy = Vec3(data[1], data[4], data[7]);
    Vec3 vecz = Vec3(data[2], data[5], data[8]);
    return Vec3(vecx.Magnitude(), vecy.Magnitude(), vecz.Magnitude());
}

void Mat3::Invert() {
    float determinant = Determinant();
    float result[9] = {0};
    result[0] = (data[4] * data[8] - data[6] * data[5]) / determinant;
    result[1] = (data[2] * data[7] - data[1] * data[8]) / determinant;
    result[2] = (data[1] * data[5] - data[2] * data[4]) / determinant;
    result[3] = (data[5] * data[6] - data[3] * data[8]) / determinant;
    result[4] = (data[0] * data[8] - data[2] * data[6]) / determinant;
    result[5] = (data[3] * data[2] - data[0] * data[5]) / determinant;
    result[6] = (data[3] * data[7] - data[6] * data[4]) / determinant;
    result[7] = (data[6] * data[1] - data[0] * data[6]) / determinant;
    result[8] = (data[0] * data[4] - data[3] * data[1]) / determinant;

    for(int i = 0; i < 9; i++) {
        data[i] = result[i];
    }
}

Mat3 Mat3::Inverted() const {
    float determinant = Determinant();
    Mat3 result = Mat3();
    result.data[0] = (data[4] * data[8] - data[6] * data[5]) / determinant;
    result.data[1] = (data[2] * data[7] - data[1] * data[8]) / determinant;
    result.data[2] = (data[1] * data[5] - data[2] * data[4]) / determinant;
    result.data[3] = (data[5] * data[6] - data[3] * data[8]) / determinant;
    result.data[4] = (data[0] * data[8] - data[2] * data[6]) / determinant;
    result.data[5] = (data[3] * data[2] - data[0] * data[5]) / determinant;
    result.data[6] = (data[3] * data[7] - data[6] * data[4]) / determinant;
    result.data[7] = (data[6] * data[1] - data[0] * data[6]) / determinant;
    result.data[8] = (data[0] * data[4] - data[3] * data[1]) / determinant;
    return result;
}

bool Mat3::IsInvertible() const {
    return Determinant() != 0;
}

/*
Euler Mat3::ToEuler() {
}

Quaternion Mat3::ToQuaternion(){
}
*/

void Mat3::Transpose() {
    std::swap(data[1], data[3]);
    std::swap(data[2], data[6]);
    std::swap(data[5], data[7]);
}

Mat3 Mat3::Transposed() const {
    Mat3 result = Mat3();
    result.data[0] = data[0];
    result.data[1] = data[3];
    result.data[2] = data[6];
    result.data[3] = data[1];
    result.data[4] = data[4];
    result.data[5] = data[7];
    result.data[6] = data[2];
    result.data[7] = data[5];
    result.data[8] = data[8];
    return result;
}

Mat3 operator*(const float& lhs, const Mat3& rhs) {
    Mat3 result = Mat3();
    for(int i = 0; i < 9; i++) {
        result.data[i] = lhs * rhs.data[i];
    }
    return result;
}

Mat3 operator*(const Mat3& lhs, const float& rhs) {
    Mat3 result = Mat3();
    for(int i = 0; i < 9; i++) {
        result.data[i] = lhs.data[i] * rhs;
    }
    return result;
}

Mat3 operator/(const Mat3& lhs, const float& rhs) {
    Mat3 result = Mat3();
    for(int i = 0; i < 9; i++) {
        result.data[i] = lhs.data[i] / rhs;
    }
    return result;
}

Mat3 operator-(const Mat3& lhs, const Mat3& rhs) {
    Mat3 result = Mat3();
    for(int i = 0; i < 9; i++) {
        result.data[i] = lhs.data[i] - rhs.data[i];
    }
    return result;
}

Mat3 operator+(const Mat3& lhs, const Mat3& rhs) {
    Mat3 result = Mat3();
    for(int i = 0; i < 9; i++) {
        result.data[i] = lhs.data[i] + rhs.data[i];
    }
    return result;
}

Mat3 operator*(const Mat3& lhs, const Mat3& rhs) {
    Mat3 result = Mat3();
    for(int row = 0; row < 3; row++) {
        for(int col = 0; col < 3; col++) {
            for(int i = 0; i < 3; i++) {
                result.data[3 * row + col] += lhs(row, i) * rhs(i, col);
            }
        }
    }
    return result;
}

Mat3& operator*=(Mat3& lhs, const float& rhs) {
    for(int i = 0; i < 9; i++) {
        lhs.data[i] *= rhs;
    }
    return lhs;
}

Mat3& operator/=(Mat3& lhs, const float& rhs) {
    for(int i = 0; i < 9; i++) {
        lhs.data[i] /= rhs;
    }
    return lhs;
}

Mat3& operator+=(Mat3& lhs, const Mat3& rhs) {
    for(int i = 0; i < 9; i++) {
        lhs.data[i] += rhs.data[i];
    }
    return lhs;
}

Mat3& operator-=(Mat3& lhs, const Mat3& rhs) {
    for(int i = 0; i < 9; i++) {
        lhs.data[i] -= rhs.data[i];
    }
    return lhs;
}

Mat3& operator*=(Mat3& lhs, const Mat3& rhs) {
    float result[9] = {0};
    for(int row = 0; row < 3; row++) {
        for(int col = 0; col < 3; col++) {
            for(int i = 0; i < 3; i++) {
                result[3 * row + col] += lhs(row, i) * rhs(i, col);
            }
        }
    }
    for(int i = 0; i < 9; i++) {
        lhs.data[i] = result[i];
    }
    return lhs;
}

Vec3 operator*(const Mat3 lhs, const Vec3& rhs) {
    Vec3 result = Vec3();
    for(int i = 0; i < 3; i++) {
        result.x += lhs.data[i] * rhs(i);
        result.y += lhs.data[3 + i] * rhs(i);
        result.z += lhs.data[6 + i] * rhs(i);
    }
    return result;
}

float Mat3::operator()(std::size_t row, std::size_t col) const {
    return data[row * 3 + col];
}

float& Mat3::operator()(std::size_t row, std::size_t col) {
    return data[row * 3 + col];
}

Vec3 Mat3::GetCol(std::size_t idx) const {
    Vec3 result = Vec3();
    result.x = data[idx];
    result.y = data[idx + 3];
    result.z = data[idx + 6];
    return result;
}

Vec3 Mat3::GetRow(std::size_t idx) const {
    Vec3 result = Vec3();
    result.x = data[idx * 3];
    result.y = data[idx * 3 + 1];
    result.z = data[idx * 3 + 2];
    return result;
}

void Mat3::SetCol(std::size_t idx, const Vec3& vec) {
    data[idx] = vec.x;
    data[idx + 3] = vec.y;
    data[idx + 6] = vec.z;
}

void Mat3::SetRow(std::size_t idx, const Vec3& vec) {
    data[idx * 3] = vec.x;
    data[idx * 3 + 1] = vec.y;
    data[idx * 3 + 2] = vec.z;
}

} // namespace Math
} // namespace Aoba