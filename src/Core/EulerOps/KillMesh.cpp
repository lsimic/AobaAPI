#include "AobaAPI/Core/EulerOps.hpp"
#include "AobaAPI/Core/Mesh.hpp"

namespace Aoba {
namespace Core {

void KillMesh(Mesh* m) {
    // not sure if this is even necessary any more. 
    // as there is no need to iterate over mesh elements
    // since all mesh elements are destroyed when the mesh and it's mempools are destroyed.

    delete m;
}

} // namespace Core
} // namespace Aoba