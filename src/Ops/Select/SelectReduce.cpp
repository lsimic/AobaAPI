#include "AobaAPI/Ops/Select.hpp"

namespace Aoba {
namespace Ops {

const SelectResult SelectReduce(Core::Mesh* m, const std::vector<Core::Vert*>& verts,
    const std::vector<Core::Edge*>& edges, const std::vector<Core::Face*>& faces, bool faceStep) {

    // mark all input edges, verts and faces as selected
    // flags
    const int32_t SELECTED_FACE = 1 << 0;
    const int32_t SELECTED_VERT = 1 << 0;
    const int32_t SELECTED_EDGE = 1 << 0;
    const int32_t DESELECTED_FACE = 1 << 1;
    const int32_t DESELECTED_VERT = 1 << 1;
    const int32_t DESELECTED_EDGE = 1 << 1;

    std::vector<Core::Vert*> selectedVerts = std::vector<Core::Vert*>();
    std::vector<Core::Edge*> selectedEdges = std::vector<Core::Edge*>();
    std::vector<Core::Face*> selectedFaces = std::vector<Core::Face*>();

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

    // find boundary verts
    std::vector<Core::Vert*> boundaryVerts = std::vector<Core::Vert*>();
    if(!faceStep) {
        // boundary vert is one which does not have all it's edges selected
        for(Core::Vert* vert : selectedVerts) {
            for(Core::Edge* edge : vert->Edges()) {
                if(!(edge->flagsIntern & SELECTED_EDGE)) {
                    boundaryVerts.push_back(vert);
                    vert->flagsIntern += DESELECTED_VERT;
                }
            }
        }
    } else {
        // boundary vert is one which does not have all it's faces selected
        std::vector<Core::Vert*> boundaryVerts = std::vector<Core::Vert*>();
        for(Core::Vert* vert : selectedVerts) {
            for(Core::Face* face : vert->Faces()) {
                if(!(face->flagsIntern & SELECTED_FACE)) {
                    boundaryVerts.push_back(vert);
                    vert->flagsIntern += DESELECTED_VERT;
                }
            }
        }
    }

    // deselect boundary vert's edges which were selected previously
    for(Core::Vert* vert : boundaryVerts) {
        for(Core::Edge* edge : vert->Edges()) {
            if(edge->flagsIntern & SELECTED_EDGE) {
                edge->flagsIntern = SELECTED_EDGE + DESELECTED_EDGE;
            }
        }
    }

    // deselect each face which had one of it's edges deselected
    for(Core::Face* face : faces) {
        for(Core::Edge* edge : face->Edges()) {
            if(edge->flagsIntern & DESELECTED_EDGE) {
                face->flagsIntern += DESELECTED_EDGE;
                break;
            }
        }
    }

    // create result, clear flags, return
    
    std::vector<Core::Vert*> resultVerts = std::vector<Core::Vert*>();
    std::vector<Core::Edge*> resultEdges = std::vector<Core::Edge*>();
    std::vector<Core::Face*> resultFaces = std::vector<Core::Face*>();

    for(Core::Vert* vert : selectedVerts) {
        if(!(vert->flagsIntern & DESELECTED_VERT)) {
            resultVerts.push_back(vert);
        }
        vert->flagsIntern = 0;
    }

    for(Core::Edge* edge : selectedEdges) {
        if(!(edge->flagsIntern & DESELECTED_EDGE)) {
            resultEdges.push_back(edge);
        }
        edge->flagsIntern = 0;
    }

    for(Core::Face* face : selectedFaces) {
        if(!(face->flagsIntern & DESELECTED_FACE)) {
            resultFaces.push_back(face);
        }
        face->flagsIntern = 0;
    }


    SelectResult result = SelectResult();
    result.edges = resultEdges;
    result.faces = resultFaces;
    result.verts = resultVerts;
    return result;
}

} // namespace Ops
} // namespace Aoba