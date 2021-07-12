#include "AobaAPI/Core/EulerOps.hpp"
#include "AobaAPI/Core/Mesh.hpp"

namespace Aoba {
namespace Core {

void MakeEdgeVert(Mesh* m, Vert* v, Edge* newe, Vert* newv) {
    // v is already in the mesh

    // add newv to the mesh.
    newv->m = m;

    // add newe to the mesh.
    newe->m = m;

    // add existing vert v as v1, newv as v2 of the edge, set loops to nullptr
    newe->v1 = v;
    newe->v2 = newv;
    newe->l = nullptr;

    // set list of edges around newv - v2.
    // since v2 is a new vert, it only has this single edge that is adjecent.
    newv->e = newe;
    newe->v2Next = newe;
    newe->v2Prev = newe;

    // set list of edges around v - v1.
    if(v->e == nullptr) {
        // no existing edges around v
        v->e = newe;
        newe->v1Prev = newe;
        newe->v1Next = newe;
    } else {
        // v already has some edges
        Edge* current = v->e;
        Edge* prev = current->Prev(v);
        v->e = newe;

        newe->v1Prev = prev;
        newe->v1Next = current;

        // must check wether v is v1 or v2 in current and previous edge
        if(current->v1 == v) {
            current->v1Prev = newe;
        } else {
            current->v2Prev = newe;
        }

        if(prev->v1 == v) {
            prev->v1Next = newe;
        } else {
            prev->v2Next = newe;
        }
    }
}

} // namespace Core
} // namespace Aoba