#include "AobaAPI/Ops/Create.hpp"

namespace Aoba {
namespace Ops {

const CreateCircleResult CreateCircle(Core::Mesh* m, unsigned vertCount, float radius) {
    CreateCircleResult result = CreateCircleResult();
    std::vector<Core::Edge*> edges = std::vector<Core::Edge*>();
    edges.reserve(vertCount);
    std::vector<Core::Vert*> verts = std::vector<Core::Vert*>();
    verts.reserve(vertCount);

    float angle_step = 2 * PI / vertCount;

    for(unsigned i = 0; i < vertCount; ++i) {
        Core::Vert* newv = new Core::Vert();
        newv->co.x = sinf(i * angle_step) * radius;
        newv->co.y = cosf(i * angle_step) * radius;
        newv->co.z = 0;
        Core::MakeVert(m, newv);
        verts.push_back(newv);
    }

    for(unsigned i = 1; i < vertCount; ++i) {
        Core::Edge* newe = new Core::Edge();
        Core::MakeEdge(verts.at(i - 1), verts.at(i), newe);
        edges.push_back(newe);
    }

    Core::Edge* newe = new Core::Edge();
    Core::MakeEdge(verts.back(), verts.at(0), newe);
    edges.push_back(newe);

    result.edges = edges;
    result.verts = verts;

    return result;
}


} // namespace Ops
} // namespace Aoba
