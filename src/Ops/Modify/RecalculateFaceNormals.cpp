#include "AobaAPI/Ops/Modify.hpp"

namespace Aoba {
namespace Ops {

void RecalculateFaceNormals(Core::Mesh* m, const std::vector<Core::Face*>& faces) {
    for(Core::Face* face : faces) {
        face->NormalUpdate();
    }
}

} // namespace Ops
} // namespace Aoba
