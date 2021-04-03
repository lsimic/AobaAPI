#include "AobaAPI/Core/EulerOps.hpp"
#include "AobaAPI/Core/Mesh.hpp"

namespace Aoba {
namespace Core {

void KillFace(Face* f) {
    FaceLoopList* current = f->loops;
    // iterate over all Face loop lists
    do {
        // iterate over all loops in looplist
        Loop* currentLoop = current->first;
        do {
            // remove loop from list of loops around an edge.
            if(currentLoop->eNext == currentLoop && currentLoop->ePrev == currentLoop) {
                // this is the only face using this edge.
                currentLoop->e->l = nullptr;
            } else {
                // the edge of this loop has more than one loop.
                currentLoop->ePrev->eNext = currentLoop->eNext;
                currentLoop->eNext->ePrev = currentLoop->ePrev;
            }
            if(currentLoop->e->l == currentLoop) {
                currentLoop->e->l = currentLoop->eNext;
            }

            Loop* toDelete = currentLoop;
            currentLoop = currentLoop->fNext;
            delete toDelete;
        } while(currentLoop != current->first);

        current = current->next;
        delete current->prev;
    } while(current != f->loops);

    // remove edge from list of edges in mesh.
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