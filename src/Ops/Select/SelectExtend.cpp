#include "AobaAPI/Ops/Select.hpp"

namespace Aoba {
namespace Ops {

const SelectResult SelectExtend(Core::Mesh* m, const std::vector<Core::Vert*>& verts,
    const std::vector<Core::Edge*>& edges, const std::vector<Core::Face*>& faces, bool faceStep) {
    // flags
    const int32_t SELECTED_FACE = 1 << 0;
    const int32_t SELECTED_VERT = 1 << 0;
    const int32_t SELECTED_EDGE = 1 << 0;

    std::vector<Core::Vert*> selectedVerts = std::vector<Core::Vert*>();
    std::vector<Core::Edge*> selectedEdges = std::vector<Core::Edge*>();
    std::vector<Core::Face*> selectedFaces = std::vector<Core::Face*>();
    std::vector<Core::Vert*> newSelectedVerts = std::vector<Core::Vert*>();
    std::vector<Core::Face*> adjecentFaces = std::vector<Core::Face*>();

    // mark input verts as selected
    for(Core::Vert* vert : verts) {
        if(!(vert->flagsIntern & SELECTED_VERT)) {
            vert->flagsIntern = SELECTED_VERT;
            selectedVerts.push_back(vert);
        }
    }

    // mark input edges and their verts as selected
    for(Core::Edge* edge : edges) {
        if(!(edge->flagsIntern & SELECTED_EDGE)) {
            edge->flagsIntern = SELECTED_EDGE;
            selectedEdges.push_back(edge);
            for(Core::Vert* vert : edge->Verts()) {
                if(!(vert->flagsIntern & SELECTED_VERT)) {
                    vert->flagsIntern = SELECTED_VERT;
                    selectedVerts.push_back(vert);
                }
            }
        }
    }

    // mark input faces, their edges and verts as selected
    for(Core::Face* face : faces) {
        if(!(face->flagsIntern & SELECTED_FACE)) {
            face->flagsIntern = SELECTED_FACE;
            selectedFaces.push_back(face);
            for(Core::Edge* edge : face->Edges()) {
                edge->flagsIntern = SELECTED_EDGE;
                selectedEdges.push_back(edge);
            }
            for(Core::Vert* vert : face->Verts()) {
                if(!(vert->flagsIntern & SELECTED_VERT)) {
                    vert->flagsIntern = SELECTED_VERT;
                    selectedVerts.push_back(vert);
                }
            }
        }
    }

    // step using edges
    for(Core::Vert* vert : selectedVerts) {
        for(Core::Edge* edge : vert->Edges()) {
            if(!(edge->flagsIntern % SELECTED_EDGE)) {
                edge->flagsIntern = SELECTED_EDGE;
                selectedEdges.push_back(edge);
                Core::Vert* other = edge->Other(vert);
                if(!(other->flagsIntern & SELECTED_VERT)) {
                    other->flagsIntern = SELECTED_VERT;
                    newSelectedVerts.push_back(other);
                }
            }
        }
        for(Core::Face* face : vert->Faces()) {
            if(!(face->flagsIntern % SELECTED_FACE)) {
                adjecentFaces.push_back(face);
            }
        }
    }

    // concat new selected verts to selected verts
    selectedVerts.insert(selectedVerts.end(), newSelectedVerts.begin(), newSelectedVerts.end());

    // check adjecent faces
    for(Core::Face* face : adjecentFaces) {
        // if using face step, mark all edges and verts as selected
        if(faceStep) {
            for(Core::Edge* edge : face->Edges()) {
                if(!(edge->flagsIntern & SELECTED_EDGE)) {
                    edge->flagsIntern = SELECTED_EDGE;
                    selectedEdges.push_back(edge);
                }
            }
            for(Core::Vert* vert : face->Verts()) {
                if(!(vert->flagsIntern & SELECTED_VERT)) {
                    vert->flagsIntern = SELECTED_VERT;
                    selectedVerts.push_back(vert);
                }
            }
        } else {
            bool selected = true;
            for(Core::Edge* edge : face->Edges()) {
                if(!(edge->flagsIntern & SELECTED_EDGE)) {
                    selected = false;
                    break;
                }
            }
            if(selected) {
                selectedFaces.push_back(face);
            }
        }
    }

    // clear flags, return
    for(Core::Edge* edge : selectedEdges) {
        edge->flagsIntern = 0;
    }
    for(Core::Face* face : selectedFaces) {
        face->flagsIntern = 0;
    }
    for(Core::Vert* vert : selectedVerts) {
        vert->flagsIntern = 0;
    }

    SelectResult result = SelectResult();
    result.edges = selectedEdges;
    result.verts = selectedVerts;
    result.faces = selectedFaces;

    return result;
}

} // namespace Ops
} // namespace Aoba