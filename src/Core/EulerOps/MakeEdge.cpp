#include "AobaAPI/Core/EulerOps.hpp"
#include "AobaAPI/Core/Mesh.hpp"
#include <stdexcept>

namespace Aoba {
namespace Core {

void MakeEdge(Mesh* m, Vert* v1, Vert* v2, Edge* newe) {
    // v1 and v2 are already in the mesh

    if(v1 == v2) {
        throw std::invalid_argument("Self-loop edges are not allowed");
    }

    for(Core::Edge* edge : v1->Edges()) {
        if(edge->Other(v1) == v2) {
            throw std::invalid_argument("Edge already exists between v1 and v2");
        }
    }

    // add newe to the mesh.
    // mesh might not have any edges at this point.
    if(m->edges == nullptr) {
        // empty mesh case
        m->edges = newe;
        newe->mNext = newe;
        newe->mPrev = newe;
    } else {
        // some edges already exist
        m->edges->mPrev->mNext = newe;
        newe->mPrev = m->edges->mPrev;
        m->edges->mPrev = newe;
        newe->mNext = m->edges;
        m->edges = newe;
    }

    // add verts v1 and v2 to edge, set loops to nullptr
    newe->v1 = v1;
    newe->v2 = v2;
    newe->l = nullptr;
    newe->m = v1->m;

    // set list of edges around v1.
    if(v1->e == nullptr) {
        // no existing edges around v
        v1->e = newe;
        newe->v1Prev = newe;
        newe->v1Next = newe;
    } else {
        // v already has some edges
        Edge* current = v1->e;
        Edge* prev = current->Prev(v1);
        v1->e = newe;

        newe->v1Prev = prev;
        newe->v1Next = current;

        // must check wether v1 is v1 or v2 in current and previous edge
        if(current->v1 == v1) {
            current->v1Prev = newe;
        } else {
            current->v2Prev = newe;
        }

        if(prev->v1 == v1) {
            prev->v1Next = newe;
        } else {
            prev->v2Next = newe;
        }
    }

    // set list of edges around v2.
    if(v2->e == nullptr) {
        // no existing edges around v
        v2->e = newe;
        newe->v2Prev = newe;
        newe->v2Next = newe;
    } else {
        // v already has some edges
        Edge* current = v2->e;
        Edge* prev = v2->e->Prev(v2);
        v2->e = newe;

        newe->v2Prev = prev;
        newe->v2Next = current;

        // must check wether v2 is v1 or v2 in current and previous edge
        if(current->v1 == v2) {
            current->v1Prev = newe;
        } else {
            current->v2Prev = newe;
        }

        if(prev->v1 == v2) {
            prev->v1Next = newe;
        } else {
            prev->v2Next = newe;
        }
    }

    // TODO: this likely does not work if v1 == v2
}

} // namespace Core
} // namespace Aoba