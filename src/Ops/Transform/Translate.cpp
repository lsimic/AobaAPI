#include "AobaAPI/Ops/Transform.hpp"

namespace Aoba {
namespace Ops {

void Translate(Core::Mesh* m, const std::vector<Core::Vert*>& verts, const Math::Vec3& vec) {
    for(auto it = verts.begin(); it != verts.end(); ++it) {
        // TODO: check if coordinate is a part of the mesh
        // TODO: can at one point run this in paralel
        ((*it)->co) += vec;
    }
}

} // namespace Ops
} // namespace Aoba
