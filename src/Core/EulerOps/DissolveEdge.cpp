#include "AobaAPI/Core/EulerOps.hpp"
#include "AobaAPI/Core/Mesh.hpp"

#include <stdexcept>

namespace Aoba {
namespace Core {

void DissolveEdge(Mesh* m, Edge* e, Face* fSurvivor) {
    // check if the edge is dissolvable
    // edge is not dissolvable if
    // 1. it is not manifold
    if(e->Loops().size() != 2 || !(e->IsManifold())) {
        throw std::invalid_argument("Edge must have two adjecent faces and must be manifold.");
    }
    // get the ohter face and loops around this edge
    Face* other;
    Loop* survLoop;
    Loop* otherLoop;
    if(e->l->f == fSurvivor) {
        survLoop = e->l;
        otherLoop = e->l->eNext;
        other = e->l->eNext->f;
    } else {
        other = e->l->f;
        survLoop = e->l->eNext;
        otherLoop = e->l;
    }

    // 2. there exists another edge which is a boundary between the same two faces
    Loop* faceLoop = survLoop->fNext;
    do {
        Loop* edgeLoop = faceLoop;
        do {
            if(edgeLoop->f == other) {
                throw std::invalid_argument("Another shared edge exists between the two faces.");
            }
            edgeLoop = edgeLoop->eNext;
        } while(edgeLoop != faceLoop);
        faceLoop = faceLoop->fNext;
    } while(faceLoop != survLoop);

    // replace fnext/fprev pointers inside both loops
    Loop* otherNext = otherLoop->fNext;
    Loop* otherPrev = otherLoop->fPrev;
    Loop* survNext = survLoop->fNext;
    Loop* survPrev = survLoop->fPrev;
    survPrev->fNext = otherNext;
    survNext->fPrev = otherPrev;
    otherNext->fPrev = survPrev;
    otherPrev->fNext = survNext;

    // make the loop point to surviving face
    Loop* current = survPrev;
    do {
        current->f = fSurvivor;
        current = current->fNext;
    } while(current != survNext);

    // make sure that the face won't point to the deleted loop
    fSurvivor->l = survPrev;

    // kill the dissolved edge
    e->l = nullptr;
    KillEdge(m, e);
    // delete unused face loops.
    m->loopPool.Free(otherLoop);
    m->loopPool.Free(survLoop);

    // delete the other face
    other->mPrev->mNext = other->mNext;
    other->mNext->mPrev = other->mPrev;
    if(other->m->faces == other) {
        other->m->faces = other->mNext;
    }
    m->facePool.Free(other);

    return;
}

} // namespace Core
} // namespace Aoba