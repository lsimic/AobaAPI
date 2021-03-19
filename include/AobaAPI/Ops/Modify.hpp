#ifndef AOBA_OPS_MODIFY_HPP
#define AOBA_OPS_MODIFY_HPP

#include "AobaAPI/Core.hpp"
#include "AobaAPI/Math.hpp"

#include <vector>

namespace Aoba {
namespace Ops {

class ExtrudeEdgesResult {
  public:
    std::vector<Core::Vert*> verts;
    std::vector<Core::Edge*> horizontalEdges;
    std::vector<Core::Edge*> verticalEdges;
    std::vector<Core::Face*> faces;
};

const ExtrudeEdgesResult ExtrudeEdges(Core::Mesh* m, const std::vector<Core::Edge*>& edges);

class DuplicateResult {
  public:
    std::vector<Core::Vert*> verts;
    std::vector<Core::Face*> faces;
    std::vector<Core::Edge*> edges;
};

const DuplicateResult Duplicate(Core::Mesh* m, const std::vector<Core::Vert*>& verts,
    const std::vector<Core::Edge*>& edges, const std::vector<Core::Face*>& faces);

Core::Face* MakeFace(Core::Mesh* m, const std::vector<Core::Edge*>& edges);

} // namespace Ops
} // namespace Aoba

#endif