#include "AobaAPI/Core/EulerOps.hpp"
#include "AobaAPI/Core/Mesh.hpp"

#include <stdexcept>

namespace Aoba {
namespace Core {

void MakeFace(Loop* loop, Face* newf) {
    Mesh* m = loop->m;

    // set the first loop of the face
    newf->l = loop;
    Loop* current = loop;

    do {
        current->f = newf;
        current = current->fNext;
    } while(current != loop);

    // add newf to mesh
    if(m->faces == nullptr) {
        m->faces = newf;
        newf->mNext = newf;
        newf->mPrev = newf;
    } else {
        m->faces->mPrev->mNext = newf;
        newf->mPrev = m->faces->mPrev;
        m->faces->mPrev = newf;
        newf->mNext = m->faces;
        m->faces = newf;
    }

    // link face to mesh
    newf->m = m;
}

} // namespace Core
} // namespace Aoba