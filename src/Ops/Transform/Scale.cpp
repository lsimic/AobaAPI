#include "AobaAPI/Ops/Transform.hpp"

namespace Aoba {
namespace Ops {

void Scale(Core::Mesh* m, const std::vector<Core::Vert*>& verts, const Math::Vec3& center, const Math::Vec3& vec) {
    for(auto it = verts.begin(); it != verts.end(); ++it) {
        (*it)->co -= center;
        (*it)->co.x *= vec.x;
        (*it)->co.y *= vec.y;
        (*it)->co.z *= vec.z;
        (*it)->co += center;

        // TODO: check if coordinate is a part of the mesh
        // TODO: can at one point run this in paralel
    }
}

} // namespace Ops
} // namespace Aoba
