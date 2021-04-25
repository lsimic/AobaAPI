#include "AobaAPI/Core/EulerOps.hpp"
#include "AobaAPI/Core/Mesh.hpp"

namespace Aoba {
namespace Core {

void KillFace(Face* f) {
    // iterate over all face loops
    Loop* currentLoop = f->l;
    do {
        // remove loop from list of loops around an edge.
        if(currentLoop->eNext == currentLoop && currentLoop->ePrev == currentLoop) {
            // this is the only face using this edge.
            currentLoop->e->l = nullptr;
        } else {
            // the edge of this loop has more than one loop.
            currentLoop->ePrev->eNext = currentLoop->eNext;
            currentLoop->eNext->ePrev = currentLoop->ePrev;
            if(currentLoop->e->l == currentLoop) {
                currentLoop->e->l = currentLoop->eNext;
            }
        }

        Loop* toDelete = currentLoop;
        currentLoop = currentLoop->fNext;
        delete toDelete;
    } while(currentLoop != f->l);

    // remove face from list of faces in mesh.
    if(f->mNext == f && f->mPrev == f) {
        f->m->faces = nullptr;
    } else {
        f->mPrev->mNext = f->mNext;
        f->mNext->mPrev = f->mPrev;
        if(f->m->faces == f) {
            f->m->faces = f->mNext;
        }
    }

    // delete face
    delete f;
}

} // namespace Core
} // namespace Aoba