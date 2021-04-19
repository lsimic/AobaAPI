#ifndef AOBA_OPS_SELECT_HPP
#define AOBA_OPS_SELECT_HPP

#include "AobaAPI/Core.hpp"
#include "AobaAPI/Math.hpp"

#include <vector>

namespace Aoba {
namespace Ops {

class SelectResult {
  public:
    std::vector<Core::Vert*> verts;
    std::vector<Core::Edge*> edges;
    std::vector<Core::Face*> faces;
};

const SelectResult SelectExtend(Core::Mesh* m, const std::vector<Core::Vert*>& verts,
    const std::vector<Core::Edge*>& edges, const std::vector<Core::Face*>& faces, bool faceStep);

const SelectResult SelectReduce(Core::Mesh* m, const std::vector<Core::Vert*>& verts,
    const std::vector<Core::Edge*>& edges, const std::vector<Core::Face*>& faces, bool faceStep);

const SelectResult SelectFromEdges(Core::Mesh* m, const std::vector<Core::Edge*>& edges);

const SelectResult SelectFromFaces(Core::Mesh* m, const std::vector<Core::Face*>& faces);

const SelectResult SelectFromVerts(Core::Mesh* m, const std::vector<Core::Vert*>& verts);

} // namespace Ops
} // namespace Aoba

#endif