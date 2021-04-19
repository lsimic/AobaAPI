#include "AobaAPI/Ops/Select.hpp"

namespace Aoba {
namespace Ops {

const SelectResult SelectFromFaces(Core::Mesh* m, const std::vector<Core::Face*>& faces) {
    // flags
    const int32_t SELECTED_EDGE = 1 << 0;
    const int32_t SELECTED_VERT = 1 << 0;

    std::vector<Core::Vert*> verts = std::vector<Core::Vert*>();
    std::vector<Core::Edge*> edges = std::vector<Core::Edge*>();

    // iterate over input faces and select all edges exactly once
    for(Core::Face* face : faces) {
        for(Core::Edge* edge : face->Edges()) {
            if(!(edge->flagsIntern & SELECTED_EDGE)) {
                edge->flagsIntern = SELECTED_EDGE;
                edges.push_back(edge);
            }
        }
    }

    // iterate over all edges, remove flags and select all verts exactly once
    for(Core::Edge* edge : edges) {
        for(Core::Vert* vert : edge->Verts()) {
            if(!(vert->flagsIntern & SELECTED_VERT)) {
                vert->flagsIntern = SELECTED_VERT;
                verts.push_back(vert);
            }
        }
        edge->flagsIntern = 0;
    }

    // remove flags from vert
    for(Core::Vert* vert : verts) {
        vert->flagsIntern = 0;
    }

    SelectResult result = SelectResult();
    result.edges = edges;
    result.faces = faces;
    result.verts = verts;

    return result;

    // TODO: if a face is not inside input faces, but all of its adjecemt faces are selected
    // should it be selected as well because all of it's edges are selected trough input faces
    // currently it is not selected
}

} // namespace Ops
} // namespace Aoba