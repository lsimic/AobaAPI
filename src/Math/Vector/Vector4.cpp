#include "AobaAPI/Math/Vector/Vector4.hpp"

#include <cmath>
#include <stdexcept>

namespace Aoba {
namespace Math {

Vec4::Vec4() : x(0), y(0), z(0), w(0) {
}

Vec4::Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {
}

float Vec4::Dot(const Vec4& other) const {
    return x * other.x + y * other.y + z * other.z + w * other.w;
}

bool Vec4::Equals(const Vec4& other, float epsilon) const {
    if(fabsf(x - other.x) > epsilon) {
        return false;
    }
    if(fabsf(y - other.y) > epsilon) {
        return false;
    }
    if(fabsf(z - other.z) > epsilon) {
        return false;
    }
    if(fabsf(w - other.w) > epsilon) {
        return false;
    }
    return true;
}

void Vec4::Negate() {
    x = -x;
    y = -y;
    z = -z;
    w = -w;
}

Vec4 Vec4::Negated() const {
    return Vec4(-x, -y, -z, -w);
}

void Vec4::Normalize() {
    float magnitude = Magnitude();
    x /= magnitude;
    y /= magnitude;
    z /= magnitude;
    w /= magnitude;
}

Vec4 Vec4::Normalized() const {
    float magnitude = Magnitude();
    return Vec4(x / magnitude, y / magnitude, z / magnitude, w / magnitude);
}

float Vec4::Length() const {
    return sqrtf(powf(x, 2) + powf(y, 2) + powf(z, 2) + powf(w, 2));
}

float Vec4::LengthSquared() const {
    return powf(x, 2) + powf(y, 2) + powf(z, 2) + powf(w, 2);
}

float Vec4::Magnitude() const {
    return sqrtf(powf(x, 2) + powf(y, 2) + powf(z, 2) + powf(w, 2));
}

Vec4 operator*(const float& lhs, const Vec4& rhs) {
    return Vec4(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w);
}

Vec4 operator*(const Vec4& lhs, const float& rhs) {
    return Vec4(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs);
}

Vec4 operator/(const Vec4& lhs, const float& rhs) {
    return Vec4(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs);
}

Vec4 operator+(const Vec4& lhs, const Vec4& rhs) {
    return Vec4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}

Vec4 operator-(const Vec4& lhs, const Vec4& rhs) {
    return Vec4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}

Vec4& operator*=(Vec4& lhs, const float& rhs) {
    lhs.x *= rhs;
    lhs.y *= rhs;
    lhs.z *= rhs;
    lhs.w *= rhs;
    return lhs;
}

Vec4& operator/=(Vec4& lhs, const float& rhs) {
    lhs.x /= rhs;
    lhs.y /= rhs;
    lhs.z /= rhs;
    lhs.w /= rhs;
    return lhs;
}

Vec4& operator+=(Vec4& lhs, const Vec4& rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    lhs.w += rhs.w;
    return lhs;
}

Vec4& operator-=(Vec4& lhs, const Vec4& rhs) {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    lhs.z -= rhs.z;
    lhs.w -= rhs.w;
    return lhs;
}

float Vec4::operator()(std::size_t idx) const {
    if(idx > 3) {
        // TODO: should I throw my own exception?
        throw std::domain_error("Vector index out of range. Use values in [0-3].");
    } else if(idx == 0) {
        return x;
    } else if(idx == 1) {
        return y;
    } else if(idx == 2) {
        return z;
    } else {
        return w;
    }
}

float& Vec4::operator()(std::size_t idx) {
    if(idx > 3) {
        // TODO: should I throw my own exception?
        throw std::domain_error("Vector index out of range. Use values in [0-3].");
    } else if(idx == 0) {
        return x;
    } else if(idx == 1) {
        return y;
    } else if(idx == 2) {
        return z;
    } else {
        return w;
    }
}

} // namespace Math
} // namespace Aoba