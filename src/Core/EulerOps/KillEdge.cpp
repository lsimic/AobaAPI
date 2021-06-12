#include "AobaAPI/Core/EulerOps.hpp"
#include "AobaAPI/Core/Mesh.hpp"

namespace Aoba {
namespace Core {

void KillEdge(Edge* e) {
    // Kill all faces using this edge
    if(e->l != nullptr) {
        std::vector<Face*> edgeFaces = e->Faces();
        for(Face* f : edgeFaces) {
            KillFace(f);
        }
    }

    // for v1, check if this is the only edge
    if(e->v1->e == e && e->v1Next == e && e->v1Prev == e) {
        e->v1->e = nullptr;
        e->v1Next = nullptr; // likely not needed
        e->v1Prev = nullptr; // likely not needed
    } else {
        // no, v1 has more than one edge
        // if v1 points to this edge as entry point to the list, change that to the next edge
        e->v1->e = e->v1Next;

        // remove edge from v1's list
        // in the next edge in the list, v1 might be stored as v2...
        if(e->v1Prev->v1 == e->v1) {
            e->v1Prev->v1Next = e->v1Next;
        } else {
            e->v1Prev->v2Next = e->v1Next;
        }
        if(e->v1Next->v1 == e->v1) {
            e->v1Next->v1Prev = e->v1Prev;
        } else {
            e->v1Next->v2Prev = e->v1Prev;
        }
    }

    // for v2, check if this is the only edge
    if(e->v2->e == e && e->v2Next == e && e->v2Prev == e) {
        e->v2->e = nullptr;
        e->v2Next = nullptr; // likely not needed
        e->v2Prev = nullptr; // likely not needed
    } else {
        // no, v2 has more than one edge
        // if v2 points to this edge as entry point to the list, change that to the next edge
        e->v2->e = e->v2Next;

        // remove edge from v2's list
        // in the next edge in the list, v2 might be stored as v1...
        if(e->v2Prev->v2 == e->v2) {
            e->v2Prev->v2Next = e->v2Next;
        } else {
            e->v2Prev->v1Next = e->v2Next;
        }
        if(e->v2Next->v2 == e->v2) {
            e->v2Next->v2Prev = e->v2Prev;
        } else {
            e->v2Next->v1Prev = e->v2Prev;
        }
    }
    // TODO: does this work with single vertex edges?

    // remove edge from list of edges in mesh.
    if(e->mNext == e && e->mPrev == e) {
        e->m->edges = nullptr;
    } else {
        e->mPrev->mNext = e->mNext;
        e->mNext->mPrev = e->mPrev;
        if(e->m->edges == e) {
            e->m->edges = e->mNext;
        }
    }

    delete e;
}

} // namespace Core
} // namespace Aoba