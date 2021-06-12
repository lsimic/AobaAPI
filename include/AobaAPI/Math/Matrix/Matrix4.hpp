#ifndef AOBA_MATH_MATRIX_MATRIX4_HPP
#define AOBA_MATH_MATRIX_MATRIX4_HPP

#include "AobaAPI/Math/Vector/Vector4.hpp"

#include<array>

namespace Aoba {
namespace Math {

class Mat4 {
  private:
    float data[16];

  public:
    Mat4();
    Mat4(const std::array<float, 16>& vals);

    static Mat4 Diagonal(const Vec4& vec);
    static Mat4 Identity();
    static Mat4 OrthoProjection(const Vec4& axis);
    static Mat4 Rotation(const Vec4& axis, float angle);
    static Mat4 Scale(const Vec4& axis, float factor);
    static Mat4 Translation(const Vec4& vec);
    static Mat4 Zero();

    float Determinant() const;
    bool Equals(const Mat4& other, float epsilon);
    Vec4 GetScale() const;
    Vec4 GetTranslation() const;
    void Invert();
    Mat4 Inverted() const;
    bool IsInvertible() const;
    // Euler ToEuler();
    // Quaternion ToQuaternion();
    void Transpose();
    Mat4 Transposed() const;

    friend Mat4 operator*(const float& lhs, const Mat4& rhs);
    friend Mat4 operator*(const Mat4& lhs, const float& rhs);
    friend Mat4 operator/(const Mat4& lhs, const float& rhs);
    friend Mat4 operator-(const Mat4& lhs, const Mat4& rhs);
    friend Mat4 operator+(const Mat4& lhs, const Mat4& rhs);
    friend Mat4 operator*(const Mat4& lhs, const Mat4& rhs);
    friend Mat4& operator*=(Mat4& lhs, const float& rhs);
    friend Mat4& operator/=(Mat4& lhs, const float& rhs);
    friend Mat4& operator+=(Mat4& lhs, const Mat4& rhs);
    friend Mat4& operator-=(Mat4& lhs, const Mat4& rhs);
    friend Mat4& operator*=(Mat4& lhs, const Mat4& rhs);
    friend Vec4 operator*(const Mat4 lhs, const Vec4& rhs);

    float operator()(std::size_t row, std::size_t col) const;
    float& operator()(std::size_t row, std::size_t col);

    Vec4 GetCol(std::size_t idx) const;
    Vec4 GetRow(std::size_t idx) const;
    void SetCol(std::size_t idx, const Vec4& vec);
    void SetRow(std::size_t idx, const Vec4& vec);
};

Mat4 operator*(const float& lhs, const Mat4& rhs);
Mat4 operator*(const Mat4& lhs, const float& rhs);
Mat4 operator/(const Mat4& lhs, const float& rhs);
Mat4 operator-(const Mat4& lhs, const Mat4& rhs);
Mat4 operator+(const Mat4& lhs, const Mat4& rhs);
Mat4 operator*(const Mat4& lhs, const Mat4& rhs);
Mat4& operator*=(Mat4& lhs, const float& rhs);
Mat4& operator/=(Mat4& lhs, const float& rhs);
Mat4& operator+=(Mat4& lhs, const Mat4& rhs);
Mat4& operator-=(Mat4& lhs, const Mat4& rhs);
Mat4& operator*=(Mat4& lhs, const Mat4& rhs);
Vec4 operator*(const Mat4 lhs, const Vec4& rhs);

} // namespace Math
} // namespace Aoba

#endif