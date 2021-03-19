#include "AobaAPI/Math/Matrix/Matrix2.hpp"

#include <algorithm>
#include <stdexcept>

namespace Aoba {
namespace Math {

Mat2::Mat2() {
    data[0] = 0;
    data[1] = 0;
    data[2] = 0;
    data[3] = 0;
}

Mat2::Mat2(const std::array<float, 4>& vals) {
    data[0] = vals.at(0);
    data[1] = vals.at(1);
    data[2] = vals.at(2);
    data[3] = vals.at(3);
}

Mat2 Mat2::Diagonal(const Vec2& vec) {
    Mat2 result = Mat2();
    result.data[0] = vec.x;
    result.data[3] = vec.y;
    return result;
}

Mat2 Mat2::Identity() {
    Mat2 result = Mat2();
    result.data[0] = 1;
    result.data[3] = 1;
    return result;
}

Mat2 Mat2::OrthoProjection(const Vec2& axis) {
    Vec2 norm = axis.Normalized();
    Mat2 result = Mat2();
    result.data[0] = powf(norm.x, 2);
    result.data[1] = norm.x * norm.y;
    result.data[2] = norm.x * norm.y;
    result.data[3] = powf(norm.y, 2);
    return result;
}

Mat2 Mat2::Rotation(float angle) {
    Mat2 result = Mat2();
    result.data[0] = cosf(angle);
    result.data[1] = -sinf(angle);
    result.data[2] = sinf(angle);
    result.data[3] = cosf(angle);
    return result;
}

Mat2 Mat2::Scale(const Vec2& axis, float factor) {
    Vec2 norm = axis.Normalized();
    Mat2 result = Mat2();
    result.data[0] = 1 + (factor - 1) * powf(norm.x, 2);
    result.data[1] = (factor - 1) * norm.x * norm.y;
    result.data[2] = (factor - 1) * norm.x * norm.y;
    result.data[3] = 1 + (factor - 1) * powf(norm.y, 2);
    return result;
}

Mat2 Mat2::Zero() {
    return Mat2();
}

float Mat2::Determinant() const {
    return data[0] * data[3] - data[1] * data[2];
}

bool Mat2::Equals(const Mat2& other, float epsilon) {
    for(int i = 0; i < 4; i++) {
        if(fabsf(data[i] - other.data[i]) > epsilon) {
            return false;
        }
    }
    return true;
}

void Mat2::Invert() {
    float determinant = Determinant();
    std::swap(data[0], data[3]);
    data[1] = -data[1];
    data[2] = -data[2];
    data[0] /= determinant;
    data[1] /= determinant;
    data[2] /= determinant;
    data[3] /= determinant;
}

Mat2 Mat2::Inverted() const {
    float determinant = Determinant();
    Mat2 result = Mat2();
    result.data[0] = data[3] / determinant;
    result.data[1] = -data[1] / determinant;
    result.data[2] = -data[2] / determinant;
    result.data[3] = data[0] / determinant;
    return result;
}

bool Mat2::IsInvertible() const {
    return Determinant() != 0;
}

void Mat2::Transpose() {
    std::swap(data[1], data[2]);
}

Mat2 Mat2::Transposed() const {
    Mat2 result = Mat2();
    result.data[0] = data[0];
    result.data[1] = data[2];
    result.data[2] = data[1];
    result.data[3] = data[3];
    return result;
}

Mat2 operator*(const float& lhs, const Mat2& rhs) {
    Mat2 result = Mat2();
    result.data[0] = lhs * rhs.data[0];
    result.data[1] = lhs * rhs.data[1];
    result.data[2] = lhs * rhs.data[2];
    result.data[3] = lhs * rhs.data[3];
    return result;
}

Mat2 operator*(const Mat2& lhs, const float& rhs) {
    Mat2 result = Mat2();
    result.data[0] = lhs.data[0] * rhs;
    result.data[1] = lhs.data[1] * rhs;
    result.data[2] = lhs.data[2] * rhs;
    result.data[3] = lhs.data[3] * rhs;
    return result;
}

Mat2 operator/(const Mat2& lhs, const float& rhs) {
    Mat2 result = Mat2();
    result.data[0] = lhs.data[0] / rhs;
    result.data[1] = lhs.data[1] / rhs;
    result.data[2] = lhs.data[2] / rhs;
    result.data[3] = lhs.data[3] / rhs;
    return result;
}

Mat2 operator-(const Mat2& lhs, const Mat2& rhs) {
    Mat2 result = Mat2();
    result.data[0] = lhs.data[0] - rhs.data[0];
    result.data[1] = lhs.data[1] - rhs.data[1];
    result.data[2] = lhs.data[2] - rhs.data[2];
    result.data[3] = lhs.data[3] - rhs.data[3];
    return result;
}

Mat2 operator+(const Mat2& lhs, const Mat2& rhs) {
    Mat2 result = Mat2();
    result.data[0] = lhs.data[0] + rhs.data[0];
    result.data[1] = lhs.data[1] + rhs.data[1];
    result.data[2] = lhs.data[2] + rhs.data[2];
    result.data[3] = lhs.data[3] + rhs.data[3];
    return result;
}

Mat2 operator*(const Mat2& lhs, const Mat2& rhs) {
    Mat2 result = Mat2();
    result.data[0] = lhs.data[0] * rhs.data[0] + lhs.data[1] * rhs.data[2];
    result.data[1] = lhs.data[0] * rhs.data[1] + lhs.data[1] * rhs.data[3];
    result.data[2] = lhs.data[2] * rhs.data[0] + lhs.data[3] * rhs.data[2];
    result.data[3] = lhs.data[2] * rhs.data[1] + lhs.data[3] * rhs.data[3];
    return result;
}

Mat2& operator*=(Mat2& lhs, const float& rhs) {
    lhs.data[0] *= rhs;
    lhs.data[1] *= rhs;
    lhs.data[2] *= rhs;
    lhs.data[3] *= rhs;
    return lhs;
}

Mat2& operator/=(Mat2& lhs, const float& rhs) {
    lhs.data[0] /= rhs;
    lhs.data[1] /= rhs;
    lhs.data[2] /= rhs;
    lhs.data[3] /= rhs;
    return lhs;
}

Mat2& operator+=(Mat2& lhs, const Mat2& rhs) {
    lhs.data[0] += rhs.data[0];
    lhs.data[1] += rhs.data[1];
    lhs.data[2] += rhs.data[2];
    lhs.data[3] += rhs.data[3];
    return lhs;
}

Mat2& operator-=(Mat2& lhs, const Mat2& rhs) {
    lhs.data[0] -= rhs.data[0];
    lhs.data[1] -= rhs.data[1];
    lhs.data[2] -= rhs.data[2];
    lhs.data[3] -= rhs.data[3];
    return lhs;
}

Mat2& operator*=(Mat2& lhs, const Mat2& rhs) { 
    float result[4] = {0};
    result[0] = lhs.data[0] * rhs.data[0] + lhs.data[1] * rhs.data[2];
    result[1] = lhs.data[0] * rhs.data[1] + lhs.data[1] * rhs.data[3];
    result[2] = lhs.data[2] * rhs.data[0] + lhs.data[3] * rhs.data[2];
    result[3] = lhs.data[2] * rhs.data[1] + lhs.data[3] * rhs.data[3];
    lhs.data[0] = result[0];
    lhs.data[0] = result[1];
    lhs.data[0] = result[2];
    lhs.data[0] = result[3];
    return lhs;
}

Vec2 operator*(const Mat2 lhs, const Vec2& rhs) {
    Vec2 result = Vec2();
    result.x = lhs.data[0] * rhs.x + lhs.data[1] * rhs.y;
    result.y = lhs.data[2] * rhs.x + lhs.data[3] * rhs.y;
    return result;
}

float Mat2::operator()(std::size_t row, std::size_t col) const {
    return data[row * 2 + col];
}

float& Mat2::operator()(std::size_t row, std::size_t col) {
    return data[row * 2 + col];
}

Vec2 Mat2::GetCol(std::size_t idx) const {
    return Vec2(data[idx], data[idx + 2]);
}

Vec2 Mat2::GetRow(std::size_t idx) const {
    return Vec2(data[idx * 2], data[idx * 2 + 1]);
}

void Mat2::SetCol(std::size_t idx, const Vec2& vec) {
    data[idx] = vec.x;
    data[idx + 2] = vec.y;
}

void Mat2::SetRow(std::size_t idx, const Vec2& vec) {
    data[idx * 2 + 0] = vec.x;
    data[idx * 2 + 1] = vec.y;
}

} // namespace Math
} // namespace Aoba