#include "AobaAPI/Ops/Modify.hpp"

namespace Aoba {
namespace Ops {

const ExtrudeFaceRegionResult ExtrudeFaceRegion(Core::Mesh* m, const std::vector<Core::Face*>& faces, bool keepOrig) {
    // flags
    const int32_t DUPLICATED_EDGE = 1 << 0;
    const int32_t INSIDE_EDGE = 1 << 1;
    const int32_t DUPLICATED_VERT = 1 << 0;
    const int32_t EXTRUDED_VERT = 1 << 1;

    std::vector<Core::Vert*> newVerts = std::vector<Core::Vert*>();
    std::size_t vertIdx = 0;
    std::vector<Core::Edge*> horizontalEdges = std::vector<Core::Edge*>();
    std::size_t edgeIdx = 0;
    std::vector<Core::Face*> horizontalFaces = std::vector<Core::Face*>();
    // keep original edges in a list to avoid traversing faces twice.
    std::vector<Core::Edge*> originalEdges = std::vector<Core::Edge*>();
    std::vector<Core::Vert*> originalVerts = std::vector<Core::Vert*>();

    // duplicate each face(and its edges, vertices, etc)
    for(Core::Face* face : faces) {
        // iterate over all loops of a face
        std::vector<Core::Edge*> loopEdges = std::vector<Core::Edge*>();
        std::vector<Core::Vert*> loopVerts = std::vector<Core::Vert*>();

        // iterate over all loops inside this face
        for(Core::Loop* loop : face->Loops()) {
            Core::Edge* loopEdge = loop->LoopEdge();

            // check if edge had already been duplicated
            if(loopEdge->flagsIntern & DUPLICATED_EDGE) {
                // multiple faces use this edge, therefore it is inside, not on the boundary
                if(!(loopEdge->flagsIntern & INSIDE_EDGE)) {
                    loopEdge->flagsIntern += INSIDE_EDGE;
                }
                // use the stored index to add the edge, vertex to loop
                loopEdges.push_back(horizontalEdges.at(loopEdge->index));
                loopVerts.push_back(newVerts.at(loop->LoopVert()->index));
            } else {
                // add to list of orignal edges
                originalEdges.push_back(loopEdge);
                // edge was not duplicated, but some verts might have been.
                Core::Vert* v1;
                Core::Vert* v2;
                Core::Edge* newe = new Core::Edge();

                if(loopEdge->V1()->flagsIntern & DUPLICATED_VERT) {
                    // v1 was duplicated
                    v1 = newVerts.at(loopEdge->V1()->index);
                } else {
                    // v1 was not duplicated, do it now
                    v1 = new Core::Vert();
                    loopEdge->V1()->index = vertIdx;
                    loopEdge->V1()->flagsIntern = DUPLICATED_VERT;
                    ++vertIdx;
                    v1->co = loopEdge->V1()->co;
                    Core::MakeVert(m, v1);
                    newVerts.push_back(v1);
                    originalVerts.push_back(loopEdge->V1());
                }

                if(loopEdge->V2()->flagsIntern & DUPLICATED_VERT) {
                    // v1 was duplicated
                    v2 = newVerts.at(loopEdge->V2()->index);
                } else {
                    // v1 was not duplicated, do it now
                    v2 = new Core::Vert();
                    loopEdge->V2()->index = vertIdx;
                    loopEdge->V2()->flagsIntern = DUPLICATED_VERT;
                    ++vertIdx;
                    v2->co = loopEdge->V2()->co;
                    Core::MakeVert(m, v2);
                    newVerts.push_back(v2);
                    originalVerts.push_back(loopEdge->V2());
                }
                // make the edge
                Core::MakeEdge(v1, v2, newe);
                horizontalEdges.push_back(newe);
                loopEdge->index = edgeIdx;
                loopEdge->flagsIntern += DUPLICATED_EDGE;
                ++edgeIdx;
                // push edge, vert to loop
                loopEdges.push_back(newe);
                if(loopEdge->V1() == loop->LoopVert()) {
                    loopVerts.push_back(v1);
                } else {
                    loopVerts.push_back(v2);
                }
            }
        }

        // create a new loop with MakeLoop using the loopverts and loopeedges to create the top face
        Core::Loop* faceLoop = new Core::Loop();
        Core::MakeLoop(loopEdges, loopVerts, faceLoop);

        // create the face using the new loop
        Core::Face* newf = new Core::Face();
        Core::MakeFace(faceLoop, newf);
        horizontalFaces.push_back(newf);
    }

    // at this point, all faces(and edges, verts) are duplicated
    // what is left is to make the boundary faces where required
    // if the edge is used two or more times, it is not extruded!
    std::vector<Core::Edge*> verticalEdges = std::vector<Core::Edge*>();
    std::size_t verticalEdgeIndex = 0;
    std::vector<Core::Face*> verticalFaces = std::vector<Core::Face*>();

    for(Core::Edge* edge : originalEdges) {
        if(!(edge->flagsIntern & INSIDE_EDGE)) {
            Core::Vert* v0 = edge->V1();
            Core::Vert* v1 = edge->V2();
            Core::Vert* v2 = newVerts.at(v1->index);
            Core::Vert* v3 = newVerts.at(v0->index);

            Core::Edge* e1;
            Core::Edge* e3;
            Core::Edge* topEdge = horizontalEdges.at(edge->index);

            if(v0->flagsIntern & EXTRUDED_VERT) {
                e3 = verticalEdges.at(v0->index);
                v3 = e3->Other(v0);
            } else {
                e3 = new Core::Edge();
                Core::MakeEdge(v0, v3, e3);
                verticalEdges.push_back(e3);
                v0->flagsIntern = EXTRUDED_VERT;
                v0->index = verticalEdgeIndex;
                ++verticalEdgeIndex;
            }
            if(v1->flagsIntern & EXTRUDED_VERT) {
                e1 = verticalEdges.at(v1->index);
                v2 = e1->Other(v1);
            } else {
                e1 = new Core::Edge();
                Core::MakeEdge(v1, v2, e1);
                verticalEdges.push_back(e1);
                v1->flagsIntern = EXTRUDED_VERT;
                v1->index = verticalEdgeIndex;
                ++verticalEdgeIndex;
            }

            // determine the orientation by the loop of the top edge (which will only have one loop)
            std::vector<Core::Vert*> loopVerts;
            std::vector<Core::Edge*> loopEdges;
            std::vector<Core::Loop*> topLoops = topEdge->Loops();
            Core::Loop* topLoop = topLoops.at(0);
            if(topLoop->LoopVert() == v3) {
                loopVerts = {v2, v3, v0, v1};
                loopEdges = {topEdge, e3, edge, e1};
            } else {
                loopVerts = {v3, v2, v1, v0};
                loopEdges = {topEdge, e1, edge, e3};
            }

            // make loop. face
            Core::Loop* newl = new Core::Loop();
            Core::Face* newf = new Core::Face();
            Core::MakeLoop(loopEdges, loopVerts, newl);
            Core::MakeFace(newl, newf);
            verticalFaces.push_back(newf);
        }
    }

    // delete or clear flags for original faces;
    if(!keepOrig) {
        for(Core::Face* face : faces) {
            Core::KillFace(face);
        }
    }

    // delete or clear flags for original edges
    for(Core::Edge* edge : originalEdges) {
        if(keepOrig) {
            edge->index = 0;
            edge->flagsIntern = 0;
        } else {
            if(edge->IsWire()) {
                Core::KillEdge(edge);
            } else {
                edge->index = 0;
                edge->flagsIntern = 0;
            }
        }
    }

    // delete or clear flags from original verts
    for(Core::Vert* vert : originalVerts) {
        if(vert->Edges().size() == 0) {
            Core::KillVert(vert);
        } else {
            vert->index = 0;
            vert->flagsIntern = 0;
        }
    }

    ExtrudeFaceRegionResult result = ExtrudeFaceRegionResult();
    result.verts = newVerts;
    result.horizontalEdges = horizontalEdges;
    result.horizontalFaces = horizontalFaces;
    result.verticalEdges = verticalEdges;
    result.verticalFaces = verticalFaces;

    return result;
    // todo: further testing on non.manifold cases
    // (tbh it wasn't tested properly even with manifold cases)
}

} // namespace Ops
} // namespace Aoba