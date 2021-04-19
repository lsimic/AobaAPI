#include "AobaAPI/Ops/Select.hpp"

namespace Aoba {
namespace Ops {

const SelectResult SelectFromEdges(Core::Mesh* m, const std::vector<Core::Edge*>& edges) {
    // flags
    const int32_t ADJECENT_FACE = 1 << 0;
    const int32_t SELECTED_VERT = 1 << 0;
    const int32_t SELECTED_EDGE = 1 << 0;

    std::vector<Core::Vert*> verts = std::vector<Core::Vert*>();
    std::vector<Core::Face*> faces = std::vector<Core::Face*>();
    std::vector<Core::Face*> adjecentFaces = std::vector<Core::Face*>();

    // iterate over all edges, mark them as selected, select verts
    // add all adjecent faces to a list
    for(Core::Edge* edge : edges) {
        edge->flagsIntern = SELECTED_EDGE;
        for(Core::Vert* vert : edge->Verts()) {
            if(!(vert->flagsIntern & SELECTED_VERT)) {
                vert->flagsIntern = SELECTED_VERT;
                verts.push_back(vert);
            }
        }
        for(Core::Face* face : edge->Faces()) {
            if(!(face->flagsIntern & ADJECENT_FACE)) {
                face->flagsIntern = ADJECENT_FACE;
                adjecentFaces.push_back(face);
            }
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

    // TODO: if an edge is not inside input edge, but some of its adjecemt edges are selected on either side
    // should it be selected as well because both of it's verts are selected trough input edges
    // currently it is not selected
}

} // namespace Ops
} // namespace Aoba