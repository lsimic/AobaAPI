#include "AobaAPI/Ops/Modify.hpp"

namespace Aoba {
namespace Ops {

void ReverseFaceOrder(Core::Mesh* m, const std::vector<Core::Face*>& faces) {
    // TODO: check if all elements belong to the mesh

    for(Core::Face* f : faces) {
        f->NormalFlip(); 
    }

}

} // namespace Ops
} // namespace Aoba
