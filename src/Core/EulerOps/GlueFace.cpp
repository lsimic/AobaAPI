#include "AobaAPI/Core/EulerOps.hpp"
#include "AobaAPI/Core/Mesh.hpp"

#include <stdexcept>

namespace Aoba {
namespace Core {

void GlueFace(Face* f1, Edge* e1, Face* f2, Edge* e2) {
    // GlueFace is implemented using the GlueVert operator
    // certain optimization could be performed, ragarding joining faces at the last step or joining loop edges

    // first must check wether the provided faces and edges are mergeable
    // order of merging is defined by e1/e2, ie loops of e1, e2
    // faces are merged in such a way that the verts of each loop are joinded together, starting from e1¸and e2

    // some verts/edges may already be shared between faces, in which case an exception will be thrown and
    // no merging will be performed

    // verify that inputs are ok

    // get loops of e1, e2, find the loop referencing f1, f2
    std::vector<Loop*> e1Loops = e1->Loops();
    std::vector<Loop*> e2Loops = e2->Loops();
    Loop* e1Loop = nullptr;
    Loop* e2Loop = nullptr;
    for(Loop* loop : e1Loops) {
        if(loop->f == f1) {
            e1Loop = loop;
            break;
        }
    }
    for(Loop* loop : e2Loops) {
        if(loop->f == f2) {
            e2Loop = loop;
            break;
        }
    }

    // check if loops were found
    // if not - specified edges do not form the face boundary
    if(e1Loop == nullptr) {
        throw std::invalid_argument("Edge e1 does not belong to boundary of face f1");
    }
    if(e2Loop == nullptr) {
        throw std::invalid_argument("Edge e2 does not belong to boundary of face f2");
    }

    // get loops of face f1, f2
    std::vector<Loop*> f1Loops = std::vector<Loop*>();
    std::vector<Loop*> f2Loops = std::vector<Loop*>();

    Loop* current = e1Loop;
    f1Loops.push_back(current);
    while(current->fNext != e1Loop) {
        current = current->fNext;
        f1Loops.push_back(current);
    }

    current = e2Loop;
    f2Loops.push_back(current);
    while(current->fNext != e2Loop) {
        current = current->fNext;
        f2Loops.push_back(current);
    }

    // first check loop sizes
    if(f1Loops.size() != f2Loops.size()) {
        throw std::invalid_argument("Faces do not have an equal number of loops");
    }

    // check for already shared edges and verts
    // if an edge or verts are shared, they cannot be merged together
    for(std::size_t i = 0; i < f1Loops.size(); ++i) {
        // if the current edge borders both f1 and f2
        // current loops must use the current edge
        std::vector<Loop*> edgeLoops = f1Loops.at(i)->e->Loops();
        for(Loop* l : edgeLoops) {
            if(l->f == f2) {
                // edge is used by the face f2 as well
                // if not used in current loop, input is invalid
                if(l != f2Loops.at(i)) {
                    throw std::invalid_argument("Invalid input, loops do not match, some edge already used");
                }
                break;
            }
        }

        // check vert faces. 
        std::vector<Face*> vertFaces = f1Loops.at(i)->v->Faces();
        for(Face* f : vertFaces) {
            // if vert is used by f2, vert must be vert of the current f2Loop
            if(f2 == f) {
                if(f1Loops.at(i) != f2Loops.at(i)) {
                    throw std::invalid_argument("Invalid input, loops do not match, some vert already used");
                }
                break;
            }
        }
    }
    
    // all inputs are valid, glue verts one by one using loops to iterate
    for(std::size_t i = 0; i < f1Loops.size(); ++i) {
        GlueVert(f1Loops.at(i)->v, f2Loops.at(i)->v);
    }
    // faces, edges, verts are killdy by the gluevert operator, nothing to do here
    return;
}

} // namespace Core
} // namespace Aoba
