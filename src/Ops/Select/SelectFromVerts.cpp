#include "AobaAPI/Ops/Select.hpp"

namespace Aoba {
namespace Ops {

const SelectResult SelectFromVerts(Core::Mesh* m, const std::vector<Core::Vert*>& verts) {
    // flags
    const int32_t ADJECENT_FACE = 1 << 0;
    const int32_t SELECTED_VERT = 1 << 0;
    const int32_t SELECTED_EDGE = 1 << 0;
    const int32_t ADJECENT_EDGE = 1 << 1;

    std::vector<Core::Edge*> edges = std::vector<Core::Edge*>();
    std::vector<Core::Edge*> adjecentEdges = std::vector<Core::Edge*>();
    std::vector<Core::Face*> faces = std::vector<Core::Face*>();
    std::vector<Core::Face*> adjecentFaces = std::vector<Core::Face*>();

    // iterate over all verts, mark them as selected
    // add all adjecent edges to a list
    for(Core::Vert* vert : verts) {
        vert->flagsIntern = SELECTED_VERT;

        for(Core::Edge* edge : vert->Edges()) {
            if(!(edge->flagsIntern & ADJECENT_EDGE)) {
                edge->flagsIntern = ADJECENT_EDGE;
                adjecentEdges.push_back(edge);
            }
        }
    }

    // check all adjecent edges
    // both verts must be selected to mark the edge as selected
    // if edge is selected, add it's adjecent faces to a list.
    for(Core::Edge* edge : adjecentEdges) {
        bool selected = true;
        for(Core::Vert* vert : edge->Verts()) {
            if(!(vert->flagsIntern & SELECTED_VERT)) {
                selected = false;
                break;
            }
        }
        if(selected) {
            edges.push_back(edge);
            edge->flagsIntern = SELECTED_EDGE;
            for(Core::Face* face : edge->Faces()) {
                if(!(face->flagsIntern & ADJECENT_FACE)) {
                    face->flagsIntern = ADJECENT_FACE;
                    adjecentFaces.push_back(face);
                }
            }
        } else {
            edge->flagsIntern = 0;
        }
    }

    // check all adjecent faces
    // all loop's edges must be selected to mark the face as selected
    // remove flags from all faces
    for(Core::Face* face : adjecentFaces) {
        bool selected = true;
        for(Core::Edge* edge : face->Edges()) {
            if(!(edge->flagsIntern & SELECTED_EDGE)) {
                selected = false;
                break;
            }
        }
        if(selected) {
            faces.push_back(face);
        }
        face->flagsIntern = 0;
    }

    // remove flags from vert
    for(Core::Vert* vert : verts) {
        vert->flagsIntern = 0;
    }

    // remove flags from edges
    for(Core::Edge* edge : edges) {
        edge->flagsIntern = 0;
    }

    SelectResult result = SelectResult();
    result.edges = edges;
    result.faces = faces;
    result.verts = verts;

    return result;
}

} // namespace Ops
} // namespace Aoba