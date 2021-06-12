#include "AobaAPI/Core/EulerOps.hpp"
#include "AobaAPI/Core/Mesh.hpp"

namespace Aoba {
namespace Core {

void JoinMesh(Mesh* m1, Mesh* m2) {
    // join verts
    if(m2->verts) {
        // make all m2 verts point to m1
        Vert* current = m2->verts;
        do {
            current->m = m1;
            current = current->mNext;
        } while(current != m2->verts);

        // merge the lists
        if(m1->verts) {
            Vert* m1Start = m1->verts;
            Vert* m1End = m1->verts->mPrev;
            Vert* m2Start = m2->verts;
            Vert* m2End = m2->verts->mPrev;
            m1Start->mPrev = m2End;
            m1End->mNext = m2Start;
            m2Start->mPrev = m1End;
            m2End->mNext = m1Start; 
        } else {
            m1->verts = m2->verts;
        }

        // reset pointer(not necessary, but just in case)
        m2->verts = nullptr;
    }

    // join edges
    if(m2->edges) {
        // make all m2 edges point to m1
        Edge* current = m2->edges;
        do {
            current->m = m1;
            current = current->mNext;
        } while(current != m2->edges);

        // merge the lists
        if(m1->verts) {
            Edge* m1Start = m1->edges;
            Edge* m1End = m1->edges->mPrev;
            Edge* m2Start = m2->edges;
            Edge* m2End = m2->edges->mPrev;
            m1Start->mPrev = m2End;
            m1End->mNext = m2Start;
            m2Start->mPrev = m1End;
            m2End->mNext = m1Start;
        } else {
            m1->edges = m2->edges;
        }

        // reset pointer(not necessary, but just in case)
        m2->edges = nullptr;
    }

    // join faces and loops
    if(m2->faces) {
        // make all m2 faces and their loops point to m1
        Face* current = m2->faces;
        do {
            Loop* currentLoop = current->l;
            while(currentLoop->fNext != current->l) {
                currentLoop->m = m1;
                currentLoop = currentLoop->fNext;
            }
            current->m = m1;
            current = current->mNext;
        } while(current != m2->faces);

        // merge the lists
        if(m1->verts) {
            Face* m1Start = m1->faces;
            Face* m1End = m1->faces->mPrev;
            Face* m2Start = m2->faces;
            Face* m2End = m2->faces->mPrev;
            m1Start->mPrev = m2End;
            m1End->mNext = m2Start;
            m2Start->mPrev = m1End;
            m2End->mNext = m1Start;
        } else {
            m1->faces = m2->faces;
        }

        // reset pointer(not necessary, but just in case)
        m2->faces = nullptr;
    }

    delete m2;
}

} // namespace Core
} // namespace Aoba
