#include "AobaAPI/Core/EulerOps.hpp"
#include "AobaAPI/Core/Mesh.hpp"

#include <stdexcept>

namespace Aoba {
namespace Core {

void MakeFace(Mesh* m, Loop* loop, Face* newf) {
    // set the first loop of the face
    newf->l = loop;
    Loop* current = loop;

    do {
        current->f = newf;
        current = current->fNext;
    } while(current != loop);

    // link face to mesh
    newf->m = m;
}

} // namespace Core
} // namespace Aoba