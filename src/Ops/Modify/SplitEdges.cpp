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

    std::vector<Core::Edge*> newEdges = std::vector<Core::Edge*>();
    newEdges.reserve(edges.size() * cuts);
    std::vector<Core::Vert*> newVerts = std::vector<Core::Vert*>();
    newVerts.reserve(edges.size() * cuts);

    for(Core::Edge* edge : edges) {
        Math::Vec3 dist = edge->V2()->co - edge->V1()->co;
        Core::Edge* edgeToSplit = edge;
        float ratioSum = 0;
        for(unsigned i = 0; i < cuts; ++i) {
            ratioSum += ratios.at(i);
            Core::Edge* newe = new Core::Edge();
            Core::Vert* newv = new Core::Vert();
            newv->co = edge->V1()->co + (ratioSum * dist);
            Core::EdgeSplit(edgeToSplit, edgeToSplit->Verts().at(1), newe, newv);
            edgeToSplit = newe;
            newEdges.push_back(newe);
            newVerts.push_back(newv);
        }
    }

    SplitEdgesResult result = SplitEdgesResult();
    result.verts = newVerts;
    result.edges = newEdges;
    return result;
}

} // namespace Ops
} // namespace Aoba