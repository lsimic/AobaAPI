#include "AobaAPI/Ops/Modify.hpp"

namespace Aoba {
namespace Ops {

const ExtrudeVertsResult ExtrudeVerts(Core::Mesh* m, const std::vector<Core::Vert*>& verts) {
    // TODO: check if all elements belong to the mesh
    // TODO: check for repeating verts

    ExtrudeVertsResult result = ExtrudeVertsResult();

    std::vector<Core::Vert*> newVerts = m->vertPool.Allocate(verts.size());
    std::vector<Core::Edge*> newEdges = m->edgePool.Allocate(verts.size());

    for(std::size_t i = 0; i < verts.size(); ++i) {
        Core::Vert* newv = newVerts.at(i);
        newv->co = verts.at(i)->co;
        Core::Edge* newe = newEdges.at(i);
        Core::MakeEdgeVert(m, verts.at(i), newe, newv);
    }

    result.verts = newVerts;
    result.edges = newEdges;

    return result;
}

} // namespace Ops
} // namespace Aoba
