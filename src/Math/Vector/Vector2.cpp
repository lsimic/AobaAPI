#include "AobaAPI/Math/Vector/Vector2.hpp"

#include <cmath>
#include <stdexcept>

namespace Aoba {
namespace Math {

Vec2::Vec2() : x(0), y(0) {
}

Vec2::Vec2(float x, float y) : x(x), y(y) {
}

Vec2::Vec2(const Vec3& vec) : x(vec.x), y(vec.y) {
}

Vec2::Vec2(const Vec4& vec) : x(vec.x), y(vec.y) {
}

float Vec2::Angle(const Vec2& other) const {
    return acosf(Dot(other) / (Magnitude() * other.Magnitude()));
}

float Vec2::AngleSigned(const Vec2& other) const {
    return atan2f(y, x) - atan2f(other.y, other.x);
}

float Vec2::Dot(const Vec2& other) const {
    return x * other.x + y * other.y;
}

bool Vec2::Equals(const Vec2& other, float epsilon) const {
    if(fabsf(x - other.x) > epsilon) {
        return false;
    }
    if(fabsf(y - other.y) > epsilon) {
        return false;
    }
    return true;
}

void Vec2::Negate() {
    x = -x;
    y = -y;
}

Vec2 Vec2::Negated() const {
    return Vec2(-x, -y);
}

void Vec2::Normalize() {
    float magnitude = Magnitude();
    x /= magnitude;
    y /= magnitude;
}

Vec2 Vec2::Normalized() const {
    float magnitude = Magnitude();
    return Vec2(x / magnitude, y / magnitude);
}

float Vec2::Length() const {
    return sqrtf(powf(x, 2) + powf(y, 2));
}

float Vec2::LengthSquared() const {
    return powf(x, 2) + powf(y, 2);
}

float Vec2::Magnitude() const {
    return sqrtf(powf(x, 2) + powf(y, 2));
}

Vec2 operator*(const float& lhs, const Vec2& rhs) {
    return Vec2(lhs * rhs.x, lhs * rhs.y);
}

Vec2 operator*(const Vec2& lhs, const float& rhs) {
    return Vec2(lhs.x * rhs, lhs.y * rhs);
}

Vec2 operator/(const Vec2& lhs, const float& rhs) {
    return Vec2(lhs.x / rhs, lhs.y / rhs);
}

Vec2 operator+(const Vec2& lhs, const Vec2& rhs) {
    return Vec2(lhs.x + rhs.x, lhs.y + rhs.y);
}

Vec2 operator-(const Vec2& lhs, const Vec2& rhs) {
    return Vec2(lhs.x - rhs.x, lhs.y - rhs.y);
}

Vec2& operator*=(Vec2& lhs, const float& rhs) {
    lhs.x *= rhs;
    lhs.y *= rhs;
    return lhs;
}

Vec2& operator/=(Vec2& lhs, const float& rhs) {
    lhs.x /= rhs;
    lhs.y /= rhs;
    return lhs;
}

Vec2& operator+=(Vec2& lhs, const Vec2& rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}

Vec2& operator-=(Vec2& lhs, const Vec2& rhs) {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    return lhs;
}

float Vec2::operator()(std::size_t idx) const {
    if(idx > 1) {
        // TODO: should I throw my own exception?
        throw std::domain_error("Vector index out of range. Use values in [0-1].");
    } else if(idx == 0) {
        return x;
    } else {
        return y;
    }
}

float& Vec2::operator()(std::size_t idx) {
    if(idx > 1) {
        // TODO: should I throw my own exception?
        throw std::domain_error("Vector index out of range. Use values in [0-1].");
    } else if(idx == 0) {
        return x;
    } else {
        return y;
    }
}

} // namespace Math
} // namespace Aoba