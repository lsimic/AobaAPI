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
    verts.reserve(edges.size());
    std::vector<Core::Face*> faces = std::vector<Core::Face*>();
    faces.reserve(edges.size());

    const int32_t EXTRUDED_VERT = 1 << 0;

    int vertIdx = 0;
    for(int i = 0; i < edges.size(); ++i) {
        std::vector<Core::Vert*> currentVerts = edges.at(i)->Verts();
        std::vector<Core::Edge*> faceEdges = std::vector<Core::Edge*>();
        faceEdges.reserve(4);

        faceEdges.push_back(edges.at(i));

        // extrude all individual verts into vertical edges
        for(int j = 0; j < 2; ++j) {
            if(currentVerts.at(j)->flagsIntern & EXTRUDED_VERT) {
                faceEdges.push_back(verticalEdges.at(currentVerts.at(j)->index));
            } else {
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

                faceEdges.push_back(newe);
            }
        }

        // create the horizontal edge, accessing the new verts by index stored on existing verts of the current edge.
        Core::Edge* newe = new Core::Edge();
        Core::MakeEdge(verts.at(currentVerts.at(0)->index), verts.at(currentVerts.at(1)->index), newe);

        horizontalEdges.push_back(newe);
        faceEdges.push_back(newe);

        // create the new face
        Core::Face* newf = new Core::Face();
        Core::MakeFace(faceEdges, newf);

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
