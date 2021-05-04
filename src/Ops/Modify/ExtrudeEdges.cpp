#include "AobaAPI/Ops/Modify.hpp"

namespace Aoba {
namespace Ops {

const ExtrudeEdgesResult ExtrudeEdges(Core::Mesh* m, const std::vector<Core::Edge*>& edges) {
    // TODO: check if all elements belong to the mesh

    ExtrudeEdgesResult result = ExtrudeEdgesResult();

    std::vector<Core::Edge*> verticalEdges = std::vector<Core::Edge*>();
    verticalEdges.reserve(edges.size());
    std::vector<Core::Edge*> horizontalEdges = std::vector<Core::Edge*>();
    horizontalEdges.reserve(edges.size());
    std::vector<Core::Vert*> verts = std::vector<Core::Vert*>();
    verts.reserve(edges.size()); // not exact cause some verts might be shared, and some not
    std::vector<Core::Face*> faces = std::vector<Core::Face*>();
    faces.reserve(edges.size());

    const int32_t EXTRUDED_VERT = 1 << 0;

    int vertIdx = 0;
    for(int i = 0; i < edges.size(); ++i) {
        std::vector<Core::Vert*> currentVerts = edges.at(i)->Verts();
        // extrude all individual verts into vertical edges ig not already extruded
        for(int j = 0; j < 2; ++j) {
            if(!(currentVerts.at(j)->flagsIntern & EXTRUDED_VERT)) {
                Core::Vert* newv = new Core::Vert();
                Core::Edge* newe = new Core::Edge();

                Core::MakeEdgeVert(currentVerts.at(j), newe, newv);
                newv->co = currentVerts.at(j)->co;

                // set flags and index on the vert.
                currentVerts.at(j)->flagsIntern = EXTRUDED_VERT;
                currentVerts.at(j)->index = vertIdx;
                vertIdx++;

                // push new edge, vert
                verticalEdges.push_back(newe);
                verts.push_back(newv);
            }
        }

        // create the horizontal edge, accessing the new verts by index stored on existing verts of the current edge.
        Core::Edge* newe = new Core::Edge();
        Core::MakeEdge(verts.at(currentVerts.at(0)->index), verts.at(currentVerts.at(1)->index), newe);
        horizontalEdges.push_back(newe);

        // find edges and verts which form the face boundary, keeping in mind existing loops
        // try to avoid discontinued normals
        std::vector<Core::Edge*> faceEdges = std::vector<Core::Edge*>();
        faceEdges.reserve(4);
        faceEdges.push_back(edges.at(i));
        Core::Vert* loopVert = currentVerts.at(0);
        if(edges.at(i)->IsBoundary()) {
            loopVert = edges.at(i)->Loops().at(0)->LoopVert();
        }
        Core::Vert* first = edges.at(i)->Other(loopVert);
        faceEdges.push_back(verticalEdges.at(loopVert->index));
        faceEdges.push_back(horizontalEdges.at(i));
        faceEdges.push_back(verticalEdges.at(first->index));
        std::vector<Core::Vert*> faceVerts = {first, loopVert, verts.at(loopVert->index), verts.at(first->index)};
        
        // create the new face
        Core::Face* newf = new Core::Face();
        Core::Loop* newl = new Core::Loop();
        Core::MakeLoop(faceEdges, faceVerts, newl);
        Core::MakeFace(newl, newf);
        faces.push_back(newf);
    }

    // clear flags and index from verts
    for(int i = 0; i < edges.size(); ++i) {
        std::vector<Core::Vert*> currentVerts = edges.at(i)->Verts();
        for(int j = 0; j < 2; ++j) {
            currentVerts.at(j)->index = 0;
            currentVerts.at(j)->flagsIntern = 0;
        }
    }

    result.faces = faces;
    result.horizontalEdges = horizontalEdges;
    result.verticalEdges = verticalEdges;
    result.verts = verts;

    return result;
}

} // namespace Ops
} // namespace Aoba
