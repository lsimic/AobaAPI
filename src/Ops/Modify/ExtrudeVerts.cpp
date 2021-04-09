#include "AobaAPI/Ops/Modify.hpp"

namespace Aoba {
namespace Ops {

const ExtrudeVertsResult ExtrudeVerts(Core::Mesh* m, const std::vector<Core::Vert*>& verts) {
    // TODO: check if all elements belong to the mesh
    // TODO: check for repeating verts

    ExtrudeVertsResult result = ExtrudeVertsResult();

    std::vector<Core::Vert*> newVerts = std::vector<Core::Vert*>();
    newVerts.reserve(verts.size());

    std::vector<Core::Edge*> newEdges = std::vector<Core::Edge*>();
    newEdges.reserve(verts.size());

    for(Core::Vert* v : verts) {
        Core::Vert* newv = new Core::Vert();
        newv->co = v->co;
        Core::Edge* newe = new Core::Edge();
        Core::MakeEdgeVert(v, newe, newv);
        newVerts.push_back(newv);
        newEdges.push_back(newe);
    }

    result.verts = newVerts;
    result.edges = newEdges;

    return result;
}

} // namespace Ops
} // namespace Aoba
