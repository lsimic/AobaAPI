#include "AobaAPI/Math/Vector/Vector3.hpp"

#include <cmath>
#include <stdexcept>

namespace Aoba {
namespace Math {

Vec3::Vec3() : x(0), y(0), z(0) {
}

Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z) {
}

Vec3::Vec3(const Vec4& vec) : x(vec.x), y(vec.y), z(vec.z) {
}

float Vec3::Angle(const Vec3& other) const {
    return acosf(Dot(other) / (Length() * other.Length()));
}

Vec3 Vec3::Cross(const Vec3& other) const {
    return Vec3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
}

float Vec3::Dot(const Vec3& other) const {
    return x * other.x + y * other.y + z * other.z;
}

bool Vec3::Equals(const Vec3& other, float epsilon) const {
    if(fabsf(x - other.x) > epsilon) {
        return false;
    }
    if(fabsf(y - other.y) > epsilon) {
        return false;
    }
    if(fabsf(z - other.z) > epsilon) {
        return false;
    }
    return true;
}

void Vec3::Negate() {
    x = -x;
    y = -y;
    z = -z;
}

Vec3 Vec3::Negated() const {
    return Vec3(-x, -y, -z);
}

void Vec3::Normalize() {
    float magnitude = Magnitude();
    x /= magnitude;
    y /= magnitude;
    z /= magnitude;
}

Vec3 Vec3::Normalized() const {
    float magnitude = Magnitude();
    return Vec3(x / magnitude, y / magnitude, z / magnitude);
}

float Vec3::Length() const {
    return sqrtf(powf(x, 2) + powf(y, 2) + powf(z, 2));
}

float Vec3::LengthSquared() const {
    return powf(x, 2) + powf(y, 2) + powf(z, 2);
}

float Vec3::Magnitude() const {
    return sqrt(powf(x, 2) + powf(y, 2) + powf(z, 2));
}

Vec3 operator*(const float& lhs, const Vec3& rhs) {
    return Vec3(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
}

Vec3 operator*(const Vec3& lhs, const float& rhs) {
    return Vec3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
}

Vec3 operator/(const Vec3& lhs, const float& rhs) {
    return Vec3(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
}

Vec3 operator+(const Vec3& lhs, const Vec3& rhs) {
    return Vec3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

Vec3 operator-(const Vec3& lhs, const Vec3& rhs) {
    return Vec3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

Vec3& operator*=(Vec3& lhs, const float& rhs) {
    lhs.x *= rhs;
    lhs.y *= rhs;
    lhs.z *= rhs;
    return lhs;
}

Vec3& operator/=(Vec3& lhs, const float& rhs) {
    lhs.x /= rhs;
    lhs.y /= rhs;
    lhs.z /= rhs;
    return lhs;
}

Vec3& operator+=(Vec3& lhs, const Vec3& rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    return lhs;
}

Vec3& operator-=(Vec3& lhs, const Vec3& rhs) {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    lhs.z -= rhs.z;
    return lhs;
}

float Vec3::operator()(std::size_t idx) const {
    if(idx > 2) {
        // TODO: should I throw my own exception?
        throw std::domain_error("Vector index out of range. Use values in [0-2].");
    } else if(idx == 0) {
        return x;
    } else if(idx == 1) {
        return y;
    } else {
        return z;
    }
}

float& Vec3::operator()(std::size_t idx) {
    if(idx > 2) {
        // TODO: should I throw my own exception?
        throw std::domain_error("Vector index out of range. Use values in [0-2].");
    } else if(idx == 0) {
        return x;
    } else if(idx == 1) {
        return y;
    } else {
        return z;
    }
}

} // namespace Math
} // namespace Aoba