#include "AobaAPI/Ops/Modify.hpp"

namespace Aoba {
namespace Ops {

Core::Face* MakeFace(Core::Mesh* m, const std::vector<Core::Edge*>& edges) {
    Core::Face* newf = new Core::Face();
    Core::MakeFace(edges, newf);
    return newf;
}

} // namespace Ops
} // namespace Aoba
