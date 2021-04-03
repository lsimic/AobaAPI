#include "AobaAPI/Core/EulerOps.hpp"
#include "AobaAPI/Core/Mesh.hpp"

namespace Aoba {
namespace Core {

void KillMesh(Mesh* m) {
    // kill all faces (therefore loops and looplists)
    if(m->faces != nullptr) {
        Face* current = m->faces;
        Face* next = current->mNext;
        do {
            next = current->mNext;
            KillFace(current);
		} while(next != m->faces);
	}
	
    // delete all edges(without killing faces as there are none)
    if(m->edges != nullptr) {
        Edge* current = m->edges;
        Edge* next = current->mNext;
        do {
            next = current->mNext;
            delete current;
        } while(next != m->edges);
    }

	// delete all verts(without deleting edges as there are none)
    if(m->verts != nullptr) {
        Vert* current = m->verts;
        Vert* next = current->mNext;
        do {
            next = current->mNext;
            delete current;
        } while(next != m->verts);
    }

    delete m;
}

} // namespace Core
} // namespace Aoba