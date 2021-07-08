#include "AobaAPI/Ops/Modify.hpp"

#include <algorithm>
#include <stdexcept>

namespace Aoba {
namespace Ops {

const SplitEdgesResult SplitEdges(
    Core::Mesh* m, const std::vector<Core::Edge*>& edges, unsigned cuts, const std::vector<float>& ratios) {
    if((std::size_t)cuts + 1 != ratios.size()) {
        throw std::invalid_argument("Invalid number of ratios given");
    }

    // todo: check that the sum of all ratios == 1
    float sum = 0;
    for(float ratio : ratios) {
        sum += ratio;
    }
    if(sum < 0.999 || sum > 1.001) {
        throw std::invalid_argument("ratios must add up to 1.0");
    }

    std::vector<Core::Edge*> newEdges = m->edgePool.Allocate(edges.size() * cuts);
    std::vector<Core::Vert*> newVerts = m->vertPool.Allocate(edges.size() * cuts);
    std::size_t newIdx = 0;

    // reserve some space for loops
    // assuming that all edges are contigous, manifold, non-wire
    m->loopPool.Reserve(edges.size() * cuts * 2);

    for(Core::Edge* edge : edges) {
        Math::Vec3 dist = edge->V2()->co - edge->V1()->co;
        Core::Edge* edgeToSplit = edge;
        float ratioSum = 0;
        for(unsigned i = 0; i < cuts; ++i) {
            ratioSum += ratios.at(i);
            Core::Edge* newe = newEdges.at(newIdx);
            Core::Vert* newv = newVerts.at(newIdx);
            newv->co = edge->V1()->co + (ratioSum * dist);
            Core::EdgeSplit(m, edgeToSplit, edgeToSplit->Verts().at(1), newe, newv);
            edgeToSplit = newe;
        }
    }

    SplitEdgesResult result = SplitEdgesResult();
    result.verts = newVerts;
    result.edges = newEdges;
    return result;
}

} // namespace Ops
} // namespace Aoba