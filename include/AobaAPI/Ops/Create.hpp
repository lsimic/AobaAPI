#ifndef AOBA_OPS_CREATE_HPP
#define AOBA_OPS_CREATE_HPP

#include "AobaAPI/Core.hpp"
#include "AobaAPI/Math.hpp"

#include <vector>

namespace Aoba {
namespace Ops {

class CreateCircleResult {
  public:
    std::vector<Core::Vert*> verts;
    std::vector<Core::Edge*> edges;
};

const CreateCircleResult CreateCircle(Core::Mesh* m, unsigned vertCount, float radius);

} // namespace Ops
} // namespace Aoba

#endif