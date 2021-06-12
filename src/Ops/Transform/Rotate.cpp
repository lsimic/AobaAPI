#include "AobaAPI/Ops/Transform.hpp"

namespace Aoba {
namespace Ops {

void Rotate(Core::Mesh* m, const std::vector<Core::Vert*>& verts, const Math::Vec3& center, const Math::Mat3& mat) {
    for(auto it = verts.begin(); it != verts.end(); ++it) {
        (*it)->co -= center;
        (*it)->co = mat * (*it)->co;
        (*it)->co += center;

        // TODO: check if coordinate is a part of the mesh
        // TODO: can at one point run this in paralel
    }
}

} // namespace Ops
} // namespace Aoba
