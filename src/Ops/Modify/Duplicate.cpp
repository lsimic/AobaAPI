#include "AobaAPI/Ops/Modify.hpp"

namespace Aoba {
namespace Ops {

const DuplicateResult Duplicate(Core::Mesh* m, const std::vector<Core::Vert*>& verts,
    const std::vector<Core::Edge*>& edges, const std::vector<Core::Face*>& faces) {
    const int32_t COPIED = 1 << 0;

    // duplicate verts from the list
    std::vector<Core::Vert*> newVerts = std::vector<Core::Vert*>();
    newVerts.reserve(verts.size());
    int vertIdx = 0;

    for(int i = 0; i < verts.size(); ++i) {
        Core::Vert* newv = m->vertPool.Allocate();
        Core::MakeVert(m, newv);

        newv->co = verts.at(i)->co;
        verts.at(i)->flagsIntern = COPIED;

        verts.at(i)->index = vertIdx;
        vertIdx++;

        newVerts.push_back(newv);
    }

    // duplicate edges from the list, and any verts not previously duplicated
    std::vector<Core::Edge*> newEdges = std::vector<Core::Edge*>();
    newEdges.reserve(edges.size());
    int edgeIdx = 0;

    for(int i = 0; i < edges.size(); ++i) {
        Core::Edge* newe = m->edgePool.Allocate();
        Core::Edge* current = edges.at(i);
        current->index = edgeIdx;
        current->flagsIntern = COPIED;
        edgeIdx++;

        Core::Vert* v1;
        Core::Vert* v2;

        if(current->Verts().at(0)->flagsIntern & COPIED) {
            v1 = newVerts.at(current->Verts().at(0)->index);
        } else {
            v1 = m->vertPool.Allocate();
            Core::MakeVert(m, v1);

            v1->co = current->Verts().at(0)->co;
            current->Verts().at(0)->flagsIntern = COPIED;

            current->Verts().at(0)->index = vertIdx;
            vertIdx++;
            newVerts.push_back(v1);
        }

        if(current->Verts().at(1)->flagsIntern & COPIED) {
            v2 = newVerts.at(current->Verts().at(1)->index);
        } else {
            v2 = m->vertPool.Allocate();
            Core::MakeVert(m, v2);

            v2->co = current->Verts().at(1)->co;
            current->Verts().at(1)->flagsIntern = COPIED;

            current->Verts().at(1)->index = vertIdx;
            vertIdx++;
            newVerts.push_back(v2);
        }

        Core::MakeEdge(m, v1, v2, newe);
        newEdges.push_back(newe);
    }

    // duplicate Faces from the list, and any verts and edges not previously duplicated
    std::vector<Core::Face*> newFaces = std::vector<Core::Face*>();
    newFaces.reserve(edges.size());

    for(int i = 0; i < faces.size(); ++i) {
        Core::Face* newf = m->facePool.Allocate();

        std::vector<Core::Loop*> newFaceLoops = std::vector<Core::Loop*>();

        std::vector<Core::Loop*> loops = faces.at(i)->Loops();
        std::vector<Core::Vert*> loopVerts = std::vector<Core::Vert*>();
        std::vector<Core::Edge*> loopEdges = std::vector<Core::Edge*>();

        for(int k = 0; k < loops.size(); ++k) {
            // check if loop vert is copied, push to loopverts
            if(loops.at(k)->LoopVert()->flagsIntern & COPIED) {
                loopVerts.push_back(newVerts.at(loops.at(k)->LoopVert()->index));
            } else {
                Core::Vert* newv = m->vertPool.Allocate();
                Core::MakeVert(m, newv);

                newv->co = loops.at(k)->LoopVert()->co;
                loops.at(k)->LoopVert()->flagsIntern = COPIED;
                loops.at(k)->LoopVert()->index = vertIdx;
                vertIdx++;
                newVerts.push_back(newv);
                loopVerts.push_back(newv);
            }

            // check if loop edge is copied, push to loopedges
            if(loops.at(k)->LoopEdge()->flagsIntern & COPIED) {
                loopEdges.push_back(newEdges.at(loops.at(k)->LoopEdge()->index));
            } else {
                Core::Edge* newe = m->edgePool.Allocate();
                Core::Edge* current = loops.at(k)->LoopEdge();
                current->index = edgeIdx;
                current->flagsIntern = COPIED;
                edgeIdx++;

                Core::Vert* v1;
                Core::Vert* v2;

                if(current->Verts().at(0)->flagsIntern & COPIED) {
                    v1 = newVerts.at(current->Verts().at(0)->index);
                } else {
                    v1 = m->vertPool.Allocate();
                    Core::MakeVert(m, v1);

                    v1->co = current->Verts().at(0)->co;
                    current->Verts().at(0)->flagsIntern = COPIED;

                    current->Verts().at(0)->index = vertIdx;
                    vertIdx++;
                    newVerts.push_back(v1);
                }

                if(current->Verts().at(1)->flagsIntern & COPIED) {
                    v2 = newVerts.at(current->Verts().at(1)->index);
                } else {
                    v2 = m->vertPool.Allocate();
                    Core::MakeVert(m, v2);

                    v2->co = current->Verts().at(1)->co;
                    current->Verts().at(1)->flagsIntern = COPIED;

                    current->Verts().at(1)->index = vertIdx;
                    vertIdx++;
                    newVerts.push_back(v2);
                }

                Core::MakeEdge(m, v1, v2, newe);
                newEdges.push_back(newe);
                loopEdges.push_back(newe);
            }
        }

        std::vector<Core::Loop*> newl = m->loopPool.Allocate(loopEdges.size());
        Core::MakeLoop(m, loopEdges, loopVerts, newl);

        Core::MakeFace(m, newl.at(0), newf);
        newFaces.push_back(newf);
    }

    DuplicateResult result = DuplicateResult();
    result.verts = newVerts;
    result.faces = newFaces;
    result.edges = newEdges;

    // TODO: perhaps this could be done faster
    // clear all flags and indexes
    for(int i = 0; i < verts.size(); ++i) {
        verts.at(i)->flagsIntern = 0;
        verts.at(i)->index = 0;
        vertIdx++;
    }

    for(int i = 0; i < edges.size(); ++i) {
        edges.at(i)->flagsIntern = 0;
        edges.at(i)->index = 0;

        edges.at(i)->Verts().at(0)->index = 0;
        edges.at(i)->Verts().at(0)->flagsIntern = 0;
        edges.at(i)->Verts().at(1)->index = 0;
        edges.at(i)->Verts().at(1)->flagsIntern = 0;
    }

    for(int i = 0; i < faces.size(); ++i) {
        std::vector<Core::Loop*> loops = faces.at(i)->Loops();

        for(int k = 0; k < loops.size(); ++k) {
            loops.at(k)->LoopVert()->flagsIntern = 0;
            loops.at(k)->LoopVert()->index = 0;

            if(loops.at(k)->LoopEdge()->flagsIntern) {
                loops.at(k)->LoopEdge()->Verts().at(0)->index = 0;
                loops.at(k)->LoopEdge()->Verts().at(0)->flagsIntern = 0;
                loops.at(k)->LoopEdge()->Verts().at(1)->index = 0;
                loops.at(k)->LoopEdge()->Verts().at(1)->flagsIntern = 0;
            }

            loops.at(k)->LoopEdge()->flagsIntern = 0;
            loops.at(k)->LoopEdge()->index = 0;
        }
    }

    return result;
}

} // namespace Ops
} // namespace Aoba
