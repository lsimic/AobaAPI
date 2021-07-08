#include "AobaAPI/Ops/Create.hpp"

#include <cmath>

namespace Aoba {
namespace Ops {

Core::Vert* CreateVert(Core::Mesh* m, const Math::Vec3 co) {
    Core::Vert* newv = m->vertPool.Allocate();

    Core::MakeVert(m, newv);
    newv->co = co;

    return newv;
}

} // namespace Ops
} // namespace Aoba
