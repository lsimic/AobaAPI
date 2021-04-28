#include "AobaAPI/Ops/Modify.hpp"

#include <algorithm>

namespace Aoba {
namespace Ops {

Math::Vec3 CalcEdgeNo(Math::Vec3 faceNo, Core::Loop* loop) {
    Math::Vec3 coStart = loop->LoopVert()->co;
    Math::Vec3 coEnd = loop->LoopEdge()->Other(loop->LoopVert())->co;

    return faceNo.Cross((coEnd - coStart).Normalized());
}

Math::Vec3 CalcVertOffset(Math::Vec3 no1, Math::Vec3 no2) {
    Math::Vec3 result = (no1 + no2);
    result.Normalize();

    float angle = no1.Angle(no2);
    // this will error if no1 and no2 point in the s
    if(angle < 0.0000001f) {
        return no1; // no1 and no2 are practically equal, therefore edges are paralel
    }
    if(abs(PI - angle) < 0.00001f) {
        // the edges in question are really pointy, and the vector would go towards infinity
        // TODO: what t return in this case?
        return result;
    }
    result *= abs(1 / (cosf((angle) / 2)));
    return result;
}

const InsetFacesResult InsetFaces(Core::Mesh* m, const std::vector<Core::Face*>& faces, float distance) {
    std::vector<Core::Vert*> verts = std::vector<Core::Vert*>();
    std::vector<Core::Edge*> centerEdges = std::vector<Core::Edge*>();
    std::vector<Core::Edge*> boundaryEdges = std::vector<Core::Edge*>();
    std::vector<Core::Face*> centerFaces = std::vector<Core::Face*>();
    std::vector<Core::Face*> boundaryFaces = std::vector<Core::Face*>();

    for(Core::Face* face : faces) {
        face->NormalUpdate();
        std::vector<Core::Loop*> faceLoops = face->Loops();

        // vectors for edges and faces, in the same order as original face loops
        std::vector<Core::Vert*> newVerts = std::vector<Core::Vert*>();
        std::vector<Core::Edge*> newEdges = std::vector<Core::Edge*>();

        // calculate vectors pointing from edges to center of face
        Math::Vec3 edgeNoPrev = CalcEdgeNo(face->no, faceLoops.back());
        Math::Vec3 edgeNoCurrent;

        // create inner verts and edges spanning to outer verts
        for(Core::Loop* loop : faceLoops) {
            edgeNoCurrent = CalcEdgeNo(face->no, loop);
            Core::Vert* newv = new Core::Vert();
            Core::Edge* newe = new Core::Edge();
            Core::MakeEdgeVert(loop->LoopVert(), newe, newv);

            newv->co = loop->LoopVert()->co + CalcVertOffset(edgeNoPrev, edgeNoCurrent) * distance;

            newVerts.push_back(newv);
            newEdges.push_back(newe);
            verts.push_back(newv);
            boundaryEdges.push_back(newe);
            edgeNoPrev = edgeNoCurrent;
        }

        // create inner edge loop using the existing verts
        // create outer ring faces
        std::vector<Core::Edge*> innerEdges = std::vector<Core::Edge*>();
        for(std::size_t i = 0; i < newVerts.size() - 1; ++i) {
            Core::Edge* newe = new Core::Edge();
            Core::MakeEdge(newVerts.at(i), newVerts.at(i + 1), newe);
            innerEdges.push_back(newe);
            centerEdges.push_back(newe);

            // make the face as well
            std::vector<Core::Vert*> fVerts = {
                faceLoops.at(i)->LoopVert(), faceLoops.at(i + 1)->LoopVert(), newVerts.at(i + 1), newVerts.at(i)};
            std::vector<Core::Edge*> fEdges = {faceLoops.at(i)->LoopEdge(), newEdges.at(i + 1), newe, newEdges.at(i)};

            Core::Loop* newl = new Core::Loop();
            Core::MakeLoop(fEdges, fVerts, newl);
            Core::Face* newf = new Core::Face();
            Core::MakeFace(newl, newf);
            boundaryFaces.push_back(newf);
        }
        // final iteration
        Core::Edge* newe = new Core::Edge();
        Core::MakeEdge(newVerts.back(), newVerts.at(0), newe);
        innerEdges.push_back(newe);
        centerEdges.push_back(newe);
        std::vector<Core::Vert*> fVerts = {
            faceLoops.back()->LoopVert(), faceLoops.at(0)->LoopVert(), newVerts.at(0), newVerts.back()};
        std::vector<Core::Edge*> fEdges = {faceLoops.back()->LoopEdge(), newEdges.at(0), newe, newEdges.back()};

        Core::Loop* newl = new Core::Loop();
        Core::MakeLoop(fEdges, fVerts, newl);
        Core::Face* newf = new Core::Face();
        Core::MakeFace(newl, newf);
        boundaryFaces.push_back(newf);

        // create inner face
        newl = new Core::Loop();
        Core::MakeLoop(innerEdges, newVerts, newl);
        newf = new Core::Face();
        Core::MakeFace(newl, newf);

        centerFaces.push_back(newf);

        // kill original face
        Core::KillFace(face);
    }

    InsetFacesResult result = InsetFacesResult();
    result.verts = verts;
    result.boundaryEdges = boundaryEdges;
    result.boundaryFaces = boundaryFaces;
    result.centerEdges = centerEdges;
    result.centerFaces = centerFaces;
    return result;
}

} // namespace Ops
} // namespace Aoba
