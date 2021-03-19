#ifndef AOBA_MATH_VECTOR_VECTOR2_HPP
#define AOBA_MATH_VECTOR_VECTOR2_HPP

#include "Vector3.hpp"

namespace Aoba {
namespace Math {

class Vec2 {
  public:
    float x;
    float y;

    Vec2();
    Vec2(float x, float y);
    explicit Vec2(const Vec3& vec);
    explicit Vec2(const Vec4& vec);

    float Angle(const Vec2& other) const;
    float AngleSigned(const Vec2& other) const;
    float Dot(const Vec2& other) const;
    bool Equals(const Vec2& other, float epsilon) const;
    void Negate();
    Vec2 Negated() const;
    void Normalize();
    Vec2 Normalized() const;
    float Length() const;
    float LengthSquared() const;
    float Magnitude() const;

    friend Vec2 operator*(const float& lhs, const Vec2& rhs);
    friend Vec2 operator*(const Vec2& lhs, const float& rhs);
    friend Vec2 operator/(const Vec2& lhs, const float& rhs);
    friend Vec2 operator+(const Vec2& lhs, const Vec2& rhs);
    friend Vec2 operator-(const Vec2& lhs, const Vec2& rhs);
    friend Vec2& operator*=(Vec2& lhs, const float& rhs);
    friend Vec2& operator/=(Vec2& lhs, const float& rhs);
    friend Vec2& operator+=(Vec2& lhs, const Vec2& rhs);
    friend Vec2& operator-=(Vec2& lhs, const Vec2& rhs);

    float operator()(std::size_t idx) const;
    float& operator()(std::size_t idx);
};

Vec2 operator*(const float& lhs, const Vec2& rhs);
Vec2 operator*(const Vec2& lhs, const float& rhs);
Vec2 operator/(const Vec2& lhs, const float& rhs);
Vec2 operator+(const Vec2& lhs, const Vec2& rhs);
Vec2 operator-(const Vec2& lhs, const Vec2& rhs);
Vec2& operator*=(Vec2& lhs, const float& rhs);
Vec2& operator/=(Vec2& lhs, const float& rhs);
Vec2& operator+=(Vec2& lhs, const Vec2& rhs);
Vec2& operator-=(Vec2& lhs, const Vec2& rhs);

} // namespace Math
} // namespace Aoba

#endif