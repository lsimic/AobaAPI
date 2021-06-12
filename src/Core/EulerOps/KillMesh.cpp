#include "AobaAPI/Core/EulerOps.hpp"
#include "AobaAPI/Core/Mesh.hpp"

namespace Aoba {
namespace Core {

void KillMesh(Mesh* m) {
    // could be done faster, but im lazy...

    // kill all faces (therefore loops and looplists)
    if(m->faces != nullptr) {
        std::vector<Face*> faces = m->Faces();
        for(Face* f : faces) {
            KillFace(f);
        }
    }

    // delete all edges(without killing faces as there are none)
    if(m->edges != nullptr) {
        std::vector<Edge*> edges = m->Edges();
        for(Edge* e : edges) {
            KillEdge(e);
        }
    }

    // delete all verts(without deleting edges as there are none)
    if(m->verts != nullptr) {
        std::vector<Vert*> verts = m->Verts();
        for(Vert* v : verts) {
            KillVert(v);
        }
    }

    delete m;
}

} // namespace Core
} // namespace Aoba