#include "AobaAPI/Core/EulerOps.hpp"
#include "AobaAPI/Core/Mesh.hpp"


namespace Aoba {
namespace Core {

void MakeVert(Mesh* m, Vert* newv) {
    newv->e = nullptr; // new verts don't have any edges
    newv->m = m; // set pointer to mesh from vert.
}

} // namespace Core
} // namespace Aoba