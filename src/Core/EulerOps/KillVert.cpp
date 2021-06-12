#include "AobaAPI/Core/EulerOps.hpp"
#include "AobaAPI/Core/Mesh.hpp"

namespace Aoba {
namespace Core {

void KillVert(Vert* v) {
    // Kill all edges (and faces) using this edge
    std::vector<Edge*> vertEdges = v->Edges();
    if(vertEdges.size() > 0) {
        for(auto it = vertEdges.begin(); it != vertEdges.end(); ++it) {
            KillEdge(*it);
        }
    }

    // remove vert from list of verts in mesh.
    if(v->mNext == v && v->mPrev == v) {
        v->m->verts = nullptr;
    } else {
        v->mPrev->mNext = v->mNext;
        v->mNext->mPrev = v->mPrev;
        if(v->m->verts == v) {
            v->m->verts = v->mNext;
        }
    }

    delete v;
}

} // namespace Core
} // namespace Aoba