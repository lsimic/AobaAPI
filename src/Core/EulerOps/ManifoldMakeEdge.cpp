#include "AobaAPI/Core/EulerOps.hpp"
#include "AobaAPI/Core/Mesh.hpp"

#include <stdexcept>

namespace Aoba {
namespace Core {

void ManifoldMakeEdge(Vert* v1, Vert* v2, Face* f, Edge* newe, Face* newf) {
    // TODO: check wether inputs are valid

    // make the edge between v1, v2
    MakeEdge(v1, v2, newe);

    // find loops with starting point in l1, l2
    Loop* loop1 = nullptr;
    Loop* loop2 = nullptr;
    for(Core::Loop* loop : f->Loops()) {
        if(loop->v == v1) {
            loop1 = loop;
        }
        if(loop->v == v2) {
            loop2 = loop;
        }
    }
    if(loop1 == nullptr || loop2 == nullptr) {
        throw std::invalid_argument("V1 or V2 not found in face loop");
    }

    // Make two new loops
    Loop* newl1 = new Loop();
    Loop* newl2 = new Loop();
    newl1->v = v2;
    newl2->v = v1;

    // add the new loops to face loops
    newl1->fPrev = loop2->fPrev;
    newl2->fPrev = loop1->fPrev;
    newl1->fNext = loop1;
    newl2->fNext = loop2;
    loop1->fPrev->fNext = newl2;
    loop1->fPrev = newl1;
    loop2->fPrev->fNext = newl1;
    loop2->fPrev = newl2;

    // add newl1, newl2 around edge newe
    newl1->eNext = newl2;
    newl1->ePrev = newl2;
    newl2->eNext = newl1;
    newl2->ePrev = newl1;
    newl1->e = newe;
    newl2->e = newe;
    newe->l = newl1;

    // make one of the loops point to the new face.
    newf->l = newl1;
    newl1->f = newf;
    Loop* current = newl1->fNext;
    while(current != newl1) {
        current->f = newf;
        current = current->fNext;
    }
    f->l = newl2;
    newl2->f = f;
    current = newl2->fNext;
    while(current != newl2) {
        current->f = f;
        current = current->fNext;
    }

    // add newf to list of faces in mesh
    Mesh* m = f->m;
    newf->m = m;
    m->faces->mPrev->mNext = newf;
    newf->mPrev = m->faces->mPrev;
    m->faces->mPrev = newf;
    newf->mNext = m->faces;
    m->faces = newf;

    return;
}

} // namespace Core
} // namespace Aoba