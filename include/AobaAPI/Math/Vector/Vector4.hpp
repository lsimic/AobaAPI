#ifndef AOBA_MATH_VECTOR_VECTOR4_HPP
#define AOBA_MATH_VECTOR_VECTOR4_HPP

#include <cstdlib>

namespace Aoba {
namespace Math {

class Vec4 {
  public:
    float x;
    float y;
    float z;
    float w;

    Vec4();
    Vec4(float x, float y, float z, float w);

    float Dot(const Vec4& other) const;
    bool Equals(const Vec4& other, float epsilon) const;
    void Negate();
    Vec4 Negated() const;
    void Normalize();
    Vec4 Normalized() const;
    float Length() const;
    float LengthSquared() const;
    float Magnitude() const;

    friend Vec4 operator*(const float& lhs, const Vec4& rhs);
    friend Vec4 operator*(const Vec4& lhs, const float& rhs);
    friend Vec4 operator/(const Vec4& lhs, const float& rhs);
    friend Vec4 operator+(const Vec4& lhs, const Vec4& rhs);
    friend Vec4 operator-(const Vec4& lhs, const Vec4& rhs);
    friend Vec4& operator*=(Vec4& lhs, const float& rhs);
    friend Vec4& operator/=(Vec4& lhs, const float& rhs);
    friend Vec4& operator+=(Vec4& lhs, const Vec4& rhs);
    friend Vec4& operator-=(Vec4& lhs, const Vec4& rhs);

    float operator()(std::size_t idx) const;
    float& operator()(std::size_t idx);
};

Vec4 operator*(const float& lhs, const Vec4& rhs);
Vec4 operator*(const Vec4& lhs, const float& rhs);
Vec4 operator/(const Vec4& lhs, const float& rhs);
Vec4 operator+(const Vec4& lhs, const Vec4& rhs);
Vec4 operator-(const Vec4& lhs, const Vec4& rhs);
Vec4& operator*=(Vec4& lhs, const float& rhs);
Vec4& operator/=(Vec4& lhs, const float& rhs);
Vec4& operator+=(Vec4& lhs, const Vec4& rhs);
Vec4& operator-=(Vec4& lhs, const Vec4& rhs);

} // namespace Math
} // namespace Aoba

#endif
