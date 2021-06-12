#ifndef AOBA_MATH_MATRIX_MATRIX3_HPP
#define AOBA_MATH_MATRIX_MATRIX3_HPP

#include "AobaAPI/Math/Vector/Vector3.hpp"

#include <array>

namespace Aoba {
namespace Math {

class Mat3 {
  private:
    float data[9];

  public:
    Mat3();
    Mat3(const std::array<float, 9>& vals);

    static Mat3 Diagonal(const Vec3& vec);
    static Mat3 Identity();
    static Mat3 OrthoProjection(const Vec3& axis);
    static Mat3 Rotation(const Vec3& axis, float angle);
    static Mat3 Scale(const Vec3& axis, float factor);
    static Mat3 Zero();

    float Determinant() const;
    bool Equals(const Mat3& other, float epsilon);
    Vec3 GetScale() const;
    void Invert();
    Mat3 Inverted() const;
    bool IsInvertible() const;
    // Euler ToEuler();
    // Quaternion ToQuaternion();
    void Transpose();
    Mat3 Transposed() const;

    friend Mat3 operator*(const float& lhs, const Mat3& rhs);
    friend Mat3 operator*(const Mat3& lhs, const float& rhs);
    friend Mat3 operator/(const Mat3& lhs, const float& rhs);
    friend Mat3 operator-(const Mat3& lhs, const Mat3& rhs);
    friend Mat3 operator+(const Mat3& lhs, const Mat3& rhs);
    friend Mat3 operator*(const Mat3& lhs, const Mat3& rhs);
    friend Mat3& operator*=(Mat3& lhs, const float& rhs);
    friend Mat3& operator/=(Mat3& lhs, const float& rhs);
    friend Mat3& operator+=(Mat3& lhs, const Mat3& rhs);
    friend Mat3& operator-=(Mat3& lhs, const Mat3& rhs);
    friend Mat3& operator*=(Mat3& lhs, const Mat3& rhs);
    friend Vec3 operator*(const Mat3 lhs, const Vec3& rhs);

    float operator()(std::size_t row, std::size_t col) const;
    float& operator()(std::size_t row, std::size_t col);

    Vec3 GetCol(std::size_t idx) const;
    Vec3 GetRow(std::size_t idx) const;
    void SetCol(std::size_t idx, const Vec3& vec);
    void SetRow(std::size_t idx, const Vec3& vec);
};

Mat3 operator*(const float& lhs, const Mat3& rhs);
Mat3 operator*(const Mat3& lhs, const float& rhs);
Mat3 operator/(const Mat3& lhs, const float& rhs);
Mat3 operator-(const Mat3& lhs, const Mat3& rhs);
Mat3 operator+(const Mat3& lhs, const Mat3& rhs);
Mat3 operator*(const Mat3& lhs, const Mat3& rhs);
Mat3& operator*=(Mat3& lhs, const float& rhs);
Mat3& operator/=(Mat3& lhs, const float& rhs);
Mat3& operator+=(Mat3& lhs, const Mat3& rhs);
Mat3& operator-=(Mat3& lhs, const Mat3& rhs);
Mat3& operator*=(Mat3& lhs, const Mat3& rhs);
Vec3 operator*(const Mat3 lhs, const Vec3& rhs);

} // namespace Math
} // namespace Aoba

#endif