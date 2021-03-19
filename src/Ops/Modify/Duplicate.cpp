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
        Core::Vert* newv = new Core::Vert();
        Core::MakeVert(m, newv);

        newv->co = verts.at(i)->co;
        verts.at(i)->flags = COPIED;

        verts.at(i)->index = vertIdx;
        vertIdx++;

        newVerts.push_back(newv);
    }

    // duplicate edges from the list, and any verts not previously duplicated
    std::vector<Core::Edge*> newEdges = std::vector<Core::Edge*>();
    newEdges.reserve(edges.size());
    int edgeIdx = 0;

    for(int i = 0; i < edges.size(); ++i) {
        Core::Edge* newe = new Core::Edge();
        Core::Edge* current = edges.at(i);
        current->index = edgeIdx;
        current->flags = COPIED;
        edgeIdx++;

        Core::Vert* v1;
        Core::Vert* v2;

        if(current->Verts().at(0)->flags & COPIED) {
            v1 = newVerts.at(current->Verts().at(0)->index);
        } else {
            v1 = new Core::Vert();
            Core::MakeVert(m, v1);

            v1->co = current->Verts().at(0)->co;
            current->Verts().at(0)->flags = COPIED;

            current->Verts().at(0)->index = vertIdx;
            vertIdx++;
            newVerts.push_back(v1);
        }

        if(current->Verts().at(1)->flags & COPIED) {
            v2 = newVerts.at(current->Verts().at(1)->index);
        } else {
            v2 = new Core::Vert();
            Core::MakeVert(m, v2);

            v2->co = current->Verts().at(1)->co;
            current->Verts().at(1)->flags = COPIED;

            current->Verts().at(1)->index = vertIdx;
            vertIdx++;
            newVerts.push_back(v2);
        }

        Core::MakeEdge(v1, v2, newe);
        newEdges.push_back(newe);
    }

    // duplicate Faces from the list, and any verts and edges not previously duplicated
    std::vector<Core::Face*> newFaces = std::vector<Core::Face*>();
    newFaces.reserve(edges.size());

    for(int i = 0; i < faces.size(); ++i) {
        Core::Face* newf = new Core::Face();

        std::vector<Core::Loop*> newFaceLoops = std::vector<Core::Loop*>();

        std::vector<Core::FaceLoopList*> faceLoopLists = faces.at(i)->LoopLists();

        for(int j = 0; j < faceLoopLists.size(); ++j) {
            std::vector<Core::Loop*> loops = faceLoopLists.at(j)->Loops();
            std::vector<Core::Vert*> loopVerts = std::vector<Core::Vert*>();
            std::vector<Core::Edge*> loopEdges = std::vector<Core::Edge*>();

            for(int k = 0; k < loops.size(); ++k) {
                // check if loop vert is copied, push to loopverts
                if(loops.at(k)->Vert()->flags & COPIED) {
                    loopVerts.push_back(newVerts.at(loops.at(k)->Vert()->index));
                } else {
                    Core::Vert* newv = new Core::Vert();
                    Core::MakeVert(m, newv);

                    newv->co = loops.at(k)->Vert()->co;
                    loops.at(k)->Vert()->flags = COPIED;
                    loops.at(k)->Vert()->index = vertIdx;
                    vertIdx++;
                    newVerts.push_back(newv);
                    loopVerts.push_back(newv);
                }

                // check if loop edge is copied, push to loopedges
                if(loops.at(k)->Edge()->flags & COPIED) {
                    loopEdges.push_back(newEdges.at(loops.at(k)->Edge()->index));
                } else {
                    Core::Edge* newe = new Core::Edge();
                    Core::Edge* current = loops.at(k)->Edge();
                    current->index = edgeIdx;
                    current->flags = COPIED;
                    edgeIdx++;

                    Core::Vert* v1;
                    Core::Vert* v2;

                    if(current->Verts().at(0)->flags & COPIED) {
                        v1 = newVerts.at(current->Verts().at(0)->index);
                    } else {
                        v1 = new Core::Vert();
                        Core::MakeVert(m, v1);

                        v1->co = current->Verts().at(0)->co;
                        current->Verts().at(0)->flags = COPIED;

                        current->Verts().at(0)->index = vertIdx;
                        vertIdx++;
                        newVerts.push_back(v1);
                    }

                    if(current->Verts().at(1)->flags & COPIED) {
                        v2 = newVerts.at(current->Verts().at(1)->index);
                    } else {
                        v2 = new Core::Vert();
                        Core::MakeVert(m, v2);

                        v2->co = current->Verts().at(1)->co;
                        current->Verts().at(1)->flags = COPIED;

                        current->Verts().at(1)->index = vertIdx;
                        vertIdx++;
                        newVerts.push_back(v2);
                    }

                    Core::MakeEdge(v1, v2, newe);
                    newEdges.push_back(newe);
                    loopEdges.push_back(newe);
                }
            }

            Core::Loop* newl = new Core::Loop();
            Core::MakeLoop(loopEdges, loopVerts, newl);
            newFaceLoops.push_back(newl);
        }

        Core::MakeFace(newFaceLoops, newf);
        newFaces.push_back(newf);
    }

    DuplicateResult result = DuplicateResult();
    result.verts = newVerts;
    result.faces = newFaces;
    result.edges = newEdges;

    return result;
}

} // namespace Ops
} // namespace Aoba
