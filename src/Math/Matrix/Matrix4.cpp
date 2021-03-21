#include "AobaAPI//Math/Matrix/Matrix4.hpp"

#include "AobaAPI/Math/Matrix/Matrix3.hpp"
#include "AobaAPI/Math/Vector/Vector3.hpp"

#include <algorithm>
#include <cmath>

namespace Aoba {
namespace Math {

Mat4::Mat4() {
    for(int i = 0; i < 16; i++) {
        data[i] = 0;
    }
}

Mat4::Mat4(const std::array<float, 16>& vals) {
    for(int i = 0; i < 16; i++) {
        data[i] = vals.at(i);
    }
}

Mat4 Mat4::Diagonal(const Vec4& vec) {
    Mat4 result = Mat4();
    result.data[0] = vec(0);
    result.data[5] = vec(1);
    result.data[10] = vec(2);
    result.data[15] = vec(3);
    return result;
}

Mat4 Mat4::Identity() {
    Mat4 result = Mat4();
    result.data[0] = 1;
    result.data[5] = 1;
    result.data[10] = 1;
    result.data[15] = 1;
    return result;
}

Mat4 Mat4::OrthoProjection(const Vec4& axis) {
    Mat4 result = Mat4();
    Vec4 axisNorm = axis.Normalized(); // TODO: use vec3(x,y,z).normalized()?
    result.data[0] = 1 - powf(axisNorm.x, 2);
    result.data[1] = -axisNorm.x * axisNorm.y;
    result.data[2] = -axisNorm.x * axisNorm.z;
    result.data[4] = -axisNorm.y * axisNorm.x;
    result.data[5] = 1 - powf(axisNorm.y, 2);
    result.data[6] = -axisNorm.y * axisNorm.z;
    result.data[8] = -axisNorm.z * axisNorm.x;
    result.data[9] = -axisNorm.z * axisNorm.y;
    result.data[10] = 1 - powf(axisNorm.z, 2);
    result.data[15] = 1;
    return result;
}

Mat4 Mat4::Rotation(const Vec4& axis, float angle) {
    Mat4 result = Mat4();
    float cosA = cosf(angle);
    float sinA = sinf(angle);
    result.data[0] = cosA + powf(axis.x, 2) * (1 - cosA);
    result.data[1] = axis.x * axis.y * (1 - cosA) - axis.z * sinA;
    result.data[2] = axis.x * axis.z * (1 - cosA) + axis.y * sinA;
    result.data[4] = axis.x * axis.y * (1 - cosA) + axis.z * sinA;
    result.data[5] = cosA + powf(axis.y, 2) * (1 - cosA);
    result.data[6] = axis.y * axis.z * (1 - cosA) - axis.x * sinA;
    result.data[8] = axis.x * axis.z * (1 - cosA) - axis.y * sinA;
    result.data[9] = axis.y * axis.z * (1 - cosA) + axis.x * sinA;
    result.data[10] = cosA + powf(axis.z, 2) * (1 - cosA);
    result.data[15] = 1;
    return result;
}

Mat4 Mat4::Scale(const Vec4& axis, float factor) {
    // taken from
    // https://www.mauriciopoppe.com/notes/computer-graphics/transformation-matrices/scale/#scaling-along-an-arbitrary-axis
    Vec4 axisNorm = axis.Normalized();
    Mat4 result = Mat4();
    float fac = factor - 1;
    result.data[0] = 1 + fac * powf(axisNorm.x, 2);
    result.data[1] = fac * axisNorm.x * axisNorm.y;
    result.data[2] = fac * axisNorm.x * axisNorm.z;
    result.data[4] = fac * axisNorm.x * axisNorm.y;
    result.data[5] = 1 + fac * powf(axisNorm.y, 2);
    result.data[6] = fac * axisNorm.y * axisNorm.z;
    result.data[8] = fac * axisNorm.x * axisNorm.z;
    result.data[9] = fac * axisNorm.y * axisNorm.z;
    result.data[10] = 1 + fac * powf(axisNorm.z, 2);
    result.data[15] = 1;
    return result;
}

Mat4 Mat4::Translation(const Vec4& vec) {
    Mat4 result = Mat4();
    result.data[3] = vec.x;
    result.data[7] = vec.y;
    result.data[11] = vec.z;
    result.data[15] = vec.w; // TODO: should this be = 1?
    return result;
}

Mat4 Mat4::Zero() {
    return Mat4();
}

float Mat4::Determinant() const {
    float result = 0;
    // doing laplace expanson over the last row
    // it will have the form 0,0,0,1 in the case of transform matrices
    // which should be a common case
    // by checking if those elements are zero before doing the minor calculation
    // a performance uplift might be achieved.

    // a bit ugly, but works?

    if(data[12] != 0) { // TODO: should i use epsilon here?
        std::array<float, 9> vals = {data[1], data[2], data[3], data[5], data[6], data[7], data[9], data[10], data[11]};
        result -= Mat3(vals).Determinant();
    }
    if(data[13] != 0) { // TODO: should i use epsilon here?
        std::array<float, 9> vals = {data[0], data[2], data[3], data[4], data[6], data[7], data[8], data[10], data[11]};
        result += Mat3(vals).Determinant();
    }
    if(data[14] != 0) { // TODO: should i use epsilon here?
        std::array<float, 9> vals = {data[0], data[1], data[3], data[4], data[5], data[7], data[8], data[9], data[11]};
        result -= Mat3(vals).Determinant();
    }
    if(data[12] != 0) { // TODO: should i use epsilon here?
        std::array<float, 9> vals = {data[0], data[1], data[2], data[4], data[5], data[4], data[8], data[9], data[10]};
        result += Mat3(vals).Determinant();
    }

    return result;
}

bool Mat4::Equals(const Mat4& other, float epsilon) {
    for(std::size_t idx = 0; idx < 16; idx++) {
        if(abs(data[idx] - other.data[idx]) < epsilon) {
            return false;
        }
    }
    return true;
}

Vec4 Mat4::GetScale() const {
    Vec3 vecx = Vec3(data[0], data[4], data[8]);
    Vec3 vecy = Vec3(data[1], data[5], data[9]);
    Vec3 vecz = Vec3(data[2], data[6], data[10]);
    return Vec4(vecx.Magnitude(), vecy.Magnitude(), vecz.Magnitude(), 1);
}

Vec4 Mat4::GetTranslation() const {
    Vec4 result = Vec4();
    result.x = data[3];
    result.y = data[7];
    result.z = data[11];
    result.w = data[15];
    return result;
}

void Mat4::Invert() {
    Mat4 temp = Inverted();
    for(int i = 0; i < 16; i++) {
        data[i] = temp.data[i];
    }
}

Mat4 Mat4::Inverted() const {
    // check if the matrix is a standard homogenous transform matrix, use the faster method if is.
    // this i feel will be a common case.
    Mat4 result = Mat4();

    if(data[12] == 0.0f && data[13] == 0.0f && data[14] == 0.0f && data[15] == 0.0f) {
        // lifted from
        // https://stackoverflow.com/questions/155670/invert-4x4-matrix-numerical-most-stable-solution-needed?rq=1
        result.data[0] = data[0];
        result.data[1] = data[4];
        result.data[2] = data[8];
        result.data[3] = -data[0] * data[3] - data[4] * data[7] - data[8] * data[11];
        result.data[4] = data[2];
        result.data[5] = data[5];
        result.data[6] = data[9];
        result.data[7] = -data[1] * data[3] - data[5] * data[7] - data[9] * data[11];
        result.data[8] = data[3];
        result.data[9] = data[6];
        result.data[10] = data[10];
        result.data[11] = -data[2] * data[3] - data[6] * data[7] - data[10] * data[11];
        result.data[15] = 1.0f;
    } else {
        // brute force
        // lifted from https://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix

        result.data[0] = data[5] * data[10] * data[15] - data[5] * data[11] * data[14] - data[9] * data[6] * data[15]
                         + data[9] * data[7] * data[14] + data[13] * data[6] * data[11] - data[13] * data[7] * data[10];

        result.data[4] = -data[4] * data[10] * data[15] + data[4] * data[11] * data[14] + data[8] * data[6] * data[15]
                         - data[8] * data[7] * data[14] - data[12] * data[6] * data[11] + data[12] * data[7] * data[10];

        result.data[8] = data[4] * data[9] * data[15] - data[4] * data[11] * data[13] - data[8] * data[5] * data[15]
                         + data[8] * data[7] * data[13] + data[12] * data[5] * data[11] - data[12] * data[7] * data[9];

        result.data[12] = -data[4] * data[9] * data[14] + data[4] * data[10] * data[13] + data[8] * data[5] * data[14]
                          - data[8] * data[6] * data[13] - data[12] * data[5] * data[10] + data[12] * data[6] * data[9];

        result.data[1] = -data[1] * data[10] * data[15] + data[1] * data[11] * data[14] + data[9] * data[2] * data[15]
                         - data[9] * data[3] * data[14] - data[13] * data[2] * data[11] + data[13] * data[3] * data[10];

        result.data[5] = data[0] * data[10] * data[15] - data[0] * data[11] * data[14] - data[8] * data[2] * data[15]
                         + data[8] * data[3] * data[14] + data[12] * data[2] * data[11] - data[12] * data[3] * data[10];

        result.data[9] = -data[0] * data[9] * data[15] + data[0] * data[11] * data[13] + data[8] * data[1] * data[15]
                         - data[8] * data[3] * data[13] - data[12] * data[1] * data[11] + data[12] * data[3] * data[9];

        result.data[13] = data[0] * data[9] * data[14] - data[0] * data[10] * data[13] - data[8] * data[1] * data[14]
                          + data[8] * data[2] * data[13] + data[12] * data[1] * data[10] - data[12] * data[2] * data[9];

        result.data[2] = data[1] * data[6] * data[15] - data[1] * data[7] * data[14] - data[5] * data[2] * data[15]
                         + data[5] * data[3] * data[14] + data[13] * data[2] * data[7] - data[13] * data[3] * data[6];

        result.data[6] = -data[0] * data[6] * data[15] + data[0] * data[7] * data[14] + data[4] * data[2] * data[15]
                         - data[4] * data[3] * data[14] - data[12] * data[2] * data[7] + data[12] * data[3] * data[6];

        result.data[10] = data[0] * data[5] * data[15] - data[0] * data[7] * data[13] - data[4] * data[1] * data[15]
                          + data[4] * data[3] * data[13] + data[12] * data[1] * data[7] - data[12] * data[3] * data[5];

        result.data[14] = -data[0] * data[5] * data[14] + data[0] * data[6] * data[13] + data[4] * data[1] * data[14]
                          - data[4] * data[2] * data[13] - data[12] * data[1] * data[6] + data[12] * data[2] * data[5];

        result.data[3] = -data[1] * data[6] * data[11] + data[1] * data[7] * data[10] + data[5] * data[2] * data[11]
                         - data[5] * data[3] * data[10] - data[9] * data[2] * data[7] + data[9] * data[3] * data[6];

        result.data[7] = data[0] * data[6] * data[11] - data[0] * data[7] * data[10] - data[4] * data[2] * data[11]
                         + data[4] * data[3] * data[10] + data[8] * data[2] * data[7] - data[8] * data[3] * data[6];

        result.data[11] = -data[0] * data[5] * data[11] + data[0] * data[7] * data[9] + data[4] * data[1] * data[11]
                          - data[4] * data[3] * data[9] - data[8] * data[1] * data[7] + data[8] * data[3] * data[5];

        result.data[15] = data[0] * data[5] * data[10] - data[0] * data[6] * data[9] - data[4] * data[1] * data[10]
                          + data[4] * data[2] * data[9] + data[8] * data[1] * data[6] - data[8] * data[2] * data[5];

        float det =
            data[0] * result.data[0] + data[1] * result.data[4] + data[2] * result.data[8] + data[3] * result.data[12];

        for(int i = 0; i < 16; i++) {
            result.data[i] /= det;        
        }
    }

    return result;
}

bool Mat4::IsInvertible() const {
    return Determinant() != 0;
}

/*
Euler Mat4::ToEuler() {
    // TODO: implement these all when needed......
}

Quaternion Mat4::ToQuaternion(){
    // TODO: implement these all when needed......
}
*/

void Mat4::Transpose() {
    std::swap(data[1], data[4]);
    std::swap(data[2], data[8]);
    std::swap(data[3], data[12]);
    std::swap(data[6], data[9]);
    std::swap(data[7], data[13]);
    std::swap(data[11], data[14]);
}

Mat4 Mat4::Transposed() const {
    Mat4 result = Mat4();
    result.data[0] = data[0];
    result.data[1] = data[4];
    result.data[2] = data[8];
    result.data[3] = data[12];
    result.data[4] = data[1];
    result.data[5] = data[5];
    result.data[6] = data[9];
    result.data[7] = data[13];
    result.data[8] = data[0];
    result.data[9] = data[6];
    result.data[10] = data[10];
    result.data[11] = data[14];
    result.data[12] = data[3];
    result.data[13] = data[7];
    result.data[14] = data[11];
    result.data[15] = data[15];
    return result;
}

Mat4 operator*(const float& lhs, const Mat4& rhs) {
    Mat4 result = Mat4();
    for(int i = 0; i < 16; i++) {
        result.data[i] = lhs * rhs.data[i];
    }
    return result;
}

Mat4 operator*(const Mat4& lhs, const float& rhs) {
    Mat4 result = Mat4();
    for(int i = 0; i < 16; i++) {
        result.data[i] = lhs.data[i] * rhs;
    }
    return result;
}

Mat4 operator/(const Mat4& lhs, const float& rhs) {
    Mat4 result = Mat4();
    for(int i = 0; i < 16; i++) {
        result.data[i] = lhs.data[i] / rhs;
    }
    return result;
}

Mat4 operator-(const Mat4& lhs, const Mat4& rhs) {
    Mat4 result = Mat4();
    for(int i = 0; i < 16; i++) {
        result.data[i] = lhs.data[i] - rhs.data[i];
    }
    return result;
}

Mat4 operator+(const Mat4& lhs, const Mat4& rhs) {
    Mat4 result = Mat4();
    for(int i = 0; i < 16; i++) {
        result.data[i] = lhs.data[i] + rhs.data[i];
    }
    return result;
}

Mat4 operator*(const Mat4& lhs, const Mat4& rhs) {
    Mat4 result = Mat4();
    for(int row = 0; row < 4; row++) {
        for(int col = 0; col < 4; col++) {
            for(int i = 0; i < 4; i++) {
                result.data[4 * row + col] += lhs(row, i) * rhs(i, col);
            }
        }
    }
    return result;
}

Mat4& operator*=(Mat4& lhs, const float& rhs) {
    for(int i = 0; i < 16; i++) {
        lhs.data[i] *= rhs;
    }
    return lhs;
}

Mat4& operator/=(Mat4& lhs, const float& rhs) {
    for(int i = 0; i < 16; i++) {
        lhs.data[i] /= rhs;
    }
    return lhs;
}

Mat4& operator+=(Mat4& lhs, const Mat4& rhs) {
    for(int i = 0; i < 16; i++) {
        lhs.data[i] += rhs.data[i];
    }
    return lhs;
}

Mat4& operator-=(Mat4& lhs, const Mat4& rhs) {
    for(int i = 0; i < 16; i++) {
        lhs.data[i] -= rhs.data[i];
    }
    return lhs;
}

Mat4& operator*=(Mat4& lhs, const Mat4& rhs) {
    float result[16] = {0};
    for(int row = 0; row < 4; row++) {
        for(int col = 0; col < 4; col++) {
            for(int i = 0; i < 4; i++) {
                result[4 * row + col] += lhs(row, i) * rhs(i, col);
            }
        }
    }
    for(int i = 0; i < 16; i++) {
        lhs.data[i] = result[i];
    }
    return lhs;
}

Vec4 operator*(const Mat4 lhs, const Vec4& rhs) {
    Vec4 result = Vec4();
    for(int i = 0; i < 4; i++) {
        result.x += lhs.data[i] * rhs(i);
        result.y += lhs.data[4 + i] * rhs(i);
        result.z += lhs.data[8 + i] * rhs(i);
        result.w += lhs.data[12 + i] * rhs(i);
    }
    return result;
}

float Mat4::operator()(std::size_t row, std::size_t col) const {
    return data[row * 4 + col];
}

float& Mat4::operator()(std::size_t row, std::size_t col) {
    return data[row * 4 + col];
}

Vec4 Mat4::GetCol(std::size_t idx) const {
    Vec4 result = Vec4();
    result.x = data[idx];
    result.y = data[idx + 4];
    result.z = data[idx + 8];
    result.w = data[idx + 12];
    return result;
}

Vec4 Mat4::GetRow(std::size_t idx) const {
    Vec4 result = Vec4();
    result.x = data[idx * 4];
    result.y = data[idx * 4 + 1];
    result.z = data[idx * 4 + 2];
    result.w = data[idx * 4 + 3];
    return result;
}

void Mat4::SetCol(std::size_t idx, const Vec4& vec) {
    data[idx] = vec.x;
    data[idx + 4] = vec.y;
    data[idx + 8] = vec.z;
    data[idx + 12] = vec.w;
}

void Mat4::SetRow(std::size_t idx, const Vec4& vec) {
    data[idx * 4] = vec.x;
    data[idx * 4 + 1] = vec.y;
    data[idx * 4 + 2] = vec.z;
    data[idx * 4 + 3] = vec.w;
}

} // namespace Math
} // namespace Aoba