#include "AobaAPI/Ops/Transform.hpp"

namespace Aoba {
namespace Ops {

void Transform(Core::Mesh* m, const std::vector<Core::Vert*>& verts, const Math::Mat4 matrix) {
    Math::Mat3 transformMatrix = Math::Mat3();
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            transformMatrix(i, j) = matrix(i, j);
        }
    }

    for(Core::Vert* v : verts) {
        v->co = transformMatrix * v->co;
        v->co.x += matrix(0, 3);
        v->co.y += matrix(1, 3);
        v->co.z += matrix(2, 3);
    }
}

} // namespace Ops
} // namespace Aoba
