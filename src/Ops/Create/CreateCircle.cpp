#include "AobaAPI/Ops/Create.hpp"

#include <cmath>

namespace Aoba {
namespace Ops {

const CreateCircleResult CreateCircle(Core::Mesh* m, unsigned vertCount, float radius) {
    CreateCircleResult result = CreateCircleResult();
    std::vector<Core::Edge*> edges = m->edgePool.Allocate(vertCount);
    std::vector<Core::Vert*> verts = m->vertPool.Allocate(vertCount);

    float angle_step = 2 * PI / vertCount;

    for(std::size_t i = 0; i < vertCount; ++i) {
        Core::Vert* newv = verts.at(i);
        newv->co.x = sinf(i * angle_step) * radius;
        newv->co.y = cosf(i * angle_step) * radius;
        newv->co.z = 0;
        Core::MakeVert(m, newv);
    }

    for(std::size_t i = 1; i < vertCount; ++i) {
        Core::Edge* newe = edges.at(i);
        Core::MakeEdge(m, verts.at(i - 1), verts.at(i), newe);
    }

    Core::Edge* newe = edges.at(0);
    Core::MakeEdge(m, verts.back(), verts.at(0), newe);

    result.edges = edges;
    result.verts = verts;

    return result;
}


} // namespace Ops
} // namespace Aoba
