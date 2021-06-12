#ifndef AOBA_MATH_MATRIX_MATRIX2_HPP
#define AOBA_MATH_MATRIX_MATRIX2_HPP

#include "AobaAPI/Math/Vector/Vector2.hpp"

#include <array>
#include <string>

namespace Aoba {
namespace Math {

class Mat2 {
  private:
    float data[4];

  public:
    Mat2();
    Mat2(const std::array<float, 4>& vals);

    static Mat2 Diagonal(const Vec2& vec);
    static Mat2 Identity();
    static Mat2 OrthoProjection(const Vec2& axis);
    static Mat2 Rotation(float angle);
    static Mat2 Scale(const Vec2& axis, float factor);
    static Mat2 Zero();

    float Determinant() const;
    bool Equals(const Mat2& other, float epsilon);
    void Invert();
    Mat2 Inverted() const;
    bool IsInvertible() const;
    void Transpose();
    Mat2 Transposed() const;

    friend Mat2 operator*(const float& lhs, const Mat2& rhs);
    friend Mat2 operator*(const Mat2& lhs, const float& rhs);
    friend Mat2 operator/(const Mat2& lhs, const float& rhs);
    friend Mat2 operator-(const Mat2& lhs, const Mat2& rhs);
    friend Mat2 operator+(const Mat2& lhs, const Mat2& rhs);
    friend Mat2 operator*(const Mat2& lhs, const Mat2& rhs);
    friend Mat2& operator*=(Mat2& lhs, const float& rhs);
    friend Mat2& operator/=(Mat2& lhs, const float& rhs);
    friend Mat2& operator+=(Mat2& lhs, const Mat2& rhs);
    friend Mat2& operator-=(Mat2& lhs, const Mat2& rhs);
    friend Mat2& operator*=(Mat2& lhs, const Mat2& rhs);
    friend Vec2 operator*(const Mat2 lhs, const Vec2& rhs);

    float operator()(std::size_t row, std::size_t col) const;
    float& operator()(std::size_t row, std::size_t col);

    Vec2 GetCol(std::size_t idx) const;
    Vec2 GetRow(std::size_t idx) const;
    void SetCol(std::size_t idx, const Vec2& vec);
    void SetRow(std::size_t idx, const Vec2& vec);
};

Mat2 operator*(const float& lhs, const Mat2& rhs);
Mat2 operator*(const Mat2& lhs, const float& rhs);
Mat2 operator/(const Mat2& lhs, const float& rhs);
Mat2 operator-(const Mat2& lhs, const Mat2& rhs);
Mat2 operator+(const Mat2& lhs, const Mat2& rhs);
Mat2 operator*(const Mat2& lhs, const Mat2& rhs);
Mat2& operator*=(Mat2& lhs, const float& rhs);
Mat2& operator/=(Mat2& lhs, const float& rhs);
Mat2& operator+=(Mat2& lhs, const Mat2& rhs);
Mat2& operator-=(Mat2& lhs, const Mat2& rhs);
Mat2& operator*=(Mat2& lhs, const Mat2& rhs);
Vec2 operator*(const Mat2 lhs, const Vec2& rhs);

} // namespace Math
} // namespace Aoba

#endif