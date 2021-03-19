#ifndef AOBA_MATH_VECTOR_VECTOR3_HPP
#define AOBA_MATH_VECTOR_VECTOR3_HPP

#include "Vector4.hpp"

namespace Aoba {
namespace Math {

class Vec3 {
  public:
    float x;
    float y;
    float z;

    Vec3();
    Vec3(float x, float y, float z);
    explicit Vec3(const Vec4& vec);

    float Angle(const Vec3& other) const;
    Vec3 Cross(const Vec3& other) const;
    float Dot(const Vec3& other) const;
    bool Equals(const Vec3& other, float epsilon) const;
    void Negate();
    Vec3 Negated() const;
    void Normalize();
    Vec3 Normalized() const;
    float Length() const;
    float LengthSquared() const;
    float Magnitude() const;

    friend Vec3 operator*(const float& lhs, const Vec3& rhs);
    friend Vec3 operator*(const Vec3& lhs, const float& rhs);
    friend Vec3 operator/(const Vec3& lhs, const float& rhs);
    friend Vec3 operator+(const Vec3& lhs, const Vec3& rhs);
    friend Vec3 operator-(const Vec3& lhs, const Vec3& rhs);
    friend Vec3& operator*=(Vec3& lhs, const float& rhs);
    friend Vec3& operator/=(Vec3& lhs, const float& rhs);
    friend Vec3& operator+=(Vec3& lhs, const Vec3& rhs);
    friend Vec3& operator-=(Vec3& lhs, const Vec3& rhs);

    float operator()(std::size_t idx) const;
    float& operator()(std::size_t idx);
};

Vec3 operator*(const float& lhs, const Vec3& rhs);
Vec3 operator*(const Vec3& lhs, const float& rhs);
Vec3 operator/(const Vec3& lhs, const float& rhs);
Vec3 operator+(const Vec3& lhs, const Vec3& rhs);
Vec3 operator-(const Vec3& lhs, const Vec3& rhs);
Vec3& operator*=(Vec3& lhs, const float& rhs);
Vec3& operator/=(Vec3& lhs, const float& rhs);
Vec3& operator+=(Vec3& lhs, const Vec3& rhs);
Vec3& operator-=(Vec3& lhs, const Vec3& rhs);

} // namespace Math
} // namespace Aoba

#endif