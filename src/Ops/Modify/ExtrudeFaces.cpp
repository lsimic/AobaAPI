#include "AobaAPI/Ops/Modify.hpp"

namespace Aoba {
namespace Ops {

const ExtrudeFacesResult ExtrudeFaces(Core::Mesh* m, const std::vector<Core::Face*>& faces, bool keepOrig) {
    // flags
    const int32_t EXTRUDED = 1 << 0;

    // verts and edges 
    std::vector<Core::Vert*> newVerts = std::vector<Core::Vert*>();
    std::size_t newVertIdx = 0; 
    std::vector<Core::Edge*> newVerticalEdges = std::vector<Core::Edge*>(); 
    std::vector<Core::Edge*> newHorizontalEdges = std::vector<Core::Edge*>();
    std::size_t newEdgeIdx = 0; 
    std::vector<Core::Face*> newVerticalFaces = std::vector<Core::Face*>(); 
    std::vector<Core::Face*> newHorizontalFaces = std::vector<Core::Face*>(); 
     
    // iterate over all input faces
    for(Core::Face* face : faces) {
        std::vector<Core::Loop*> faceLoops = std::vector<Core::Loop*>();
        // iterate over all loops of a face using loopLists
        for(Core::FaceLoopList* loopList : face->LoopLists()) {
            std::vector<Core::Edge*> loopListEdges = std::vector<Core::Edge*>();
            std::vector<Core::Vert*> loopListVerts = std::vector<Core::Vert*>();

            // iterate over all loops inside this loop
            for(Core::Loop* loop : loopList->Loops()) {
                Core::Edge* topEdge;
                Core::Vert* v2; 
                Core::Vert* v3;
                // get the edge and verts and of current loop
                Core::Edge* loopEdge = loop->LoopEdge();
                Core::Vert* v0 = loop->LoopVert();
                Core::Vert* v1 = loopEdge->Other(v0);
                // check if the edge of current loop was already extruded
                if(loopEdge->flagsIntern & EXTRUDED) {
                    // get verts, edges using indices
                    v2 = newVerts.at(v1->index);
                    v3 = newVerts.at(v0->index);
                    topEdge = newHorizontalEdges.at(loopEdge->index);
                } else {
                    Core::Edge* leftEdge;
                    Core::Edge* rightEdge;
                    // extrude each vertex of the edge which was not extruded
                    if(v0->flagsIntern & EXTRUDED) {
                        leftEdge = newVerticalEdges.at(v0->index);
                        v3 = newVerts.at(v0->index);
                    } else {
                        leftEdge = new Core::Edge();
                        v3 = new Core::Vert();
                        v3->co = v0->co;
                        Core::MakeEdgeVert(v0, leftEdge, v3);
                        v0->index = newVertIdx;
                        v0->flagsIntern = EXTRUDED;
                        newVerts.push_back(v3);
                        newVerticalEdges.push_back(leftEdge); 
                        newVertIdx++;
                    }
                    if(v1->flagsIntern & EXTRUDED) {
                        rightEdge = newVerticalEdges.at(v1->index);
                        v2 = newVerts.at(v1->index);
                    } else {
                        rightEdge = new Core::Edge();
                        v2 = new Core::Vert();
                        v2->co = v1->co;
                        Core::MakeEdgeVert(v1, rightEdge, v2);
                        v1->index = newVertIdx;
                        v1->flagsIntern = EXTRUDED;
                        newVerts.push_back(v2);
                        newVerticalEdges.push_back(rightEdge);
                        newVertIdx++;
                    }
                    // make an edge at the top using the new vertices of the extruded edges
                    topEdge = new Core::Edge();
                    Core::MakeEdge(v3, v2, topEdge);
                    newHorizontalEdges.push_back(topEdge); 
                    loopEdge->index = newEdgeIdx;
                    loopEdge->flagsIntern = EXTRUDED;
                    newEdgeIdx++;

                    // make the extruded face
                    std::vector<Core::Edge*> loopEdges = {loopEdge, rightEdge, topEdge, leftEdge};
                    std::vector<Core::Vert*> loopVerts = {v0, v1, v2, v3};

                    Core::Loop* newl = new Core::Loop();
                    Core::Face* newf = new Core::Face();
                    Core::MakeLoop(loopEdges, loopVerts, newl);
                    Core::MakeFace(std::vector<Core::Loop*> {newl}, newf);
                    newVerticalFaces.push_back(newf);
                }

                // add the top edge and v3(same order) to the list of loop verts, edges
                loopListEdges.push_back(topEdge);
                loopListVerts.push_back(v3);
            }

            // create a new loop with MakeLoop using the loopverts and loopeedges to create the top face
            Core::Loop* faceLoop = new Core::Loop();
            Core::MakeLoop(loopListEdges, loopListVerts, faceLoop);
            // push the new loop to the list of new loops
            faceLoops.push_back(faceLoop);
        }
        // create the face using the list of new loops
        Core::Face* newf = new Core::Face();
        Core::MakeFace(faceLoops, newf);
        newHorizontalFaces.push_back(newf);
	}

    // loop over all faces again, clear indices and flags
    for(Core::Face* face : faces) {
        // iterate over all loops of a face using loopLists
        for(Core::FaceLoopList* loopList : face->LoopLists()) {
            // iterate over all loops inside this loop
            for(Core::Loop* loop : loopList->Loops()) {
                Core::Edge* edge = loop->LoopEdge();
                edge->flagsIntern = 0;
                edge->index = 0;
                edge->V1()->index = 0;
                edge->V1()->flagsIntern = 0;
                edge->V2()->index = 0;
                edge->V2()->flagsIntern = 0;
            }
        }

        // if specified, kill original face
        if(!keepOrig) {
            Core::KillFace(face);
        }
    }

    ExtrudeFacesResult result = ExtrudeFacesResult();
    result.verts = newVerts;
    result.horizontalEdges = newHorizontalEdges;
    result.horizontalFaces = newHorizontalFaces;
    result.verticalEdges = newVerticalEdges;
    result.verticalFaces = newVerticalFaces;

    return result;
}

} // namespace Core
} // namespace Aoba