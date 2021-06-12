#include "AobaAPI/Core/EulerOps.hpp"
#include "AobaAPI/Core/Mesh.hpp"


namespace Aoba {
namespace Core {

void MakeVert(Mesh* m, Vert* newv) {
    newv->e = nullptr; // new verts don't have any edges
    newv->m = m; // set pointer to mesh from vert.

    if(m->verts == nullptr) {
        // empty mesh case
        m->verts = newv;
        newv->mNext = newv;
        newv->mPrev = newv;
    } else {
        // some verts already exist
        m->verts->mPrev->mNext = newv;
        newv->mPrev = m->verts->mPrev;
        m->verts->mPrev = newv;
        newv->mNext = m->verts;
        m->verts = newv;
    }
}

} // namespace Core
} // namespace Aoba