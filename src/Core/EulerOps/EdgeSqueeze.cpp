#include "AobaAPI/Core/EulerOps.hpp"
#include "AobaAPI/Core/Mesh.hpp"

namespace Aoba {
namespace Core {

void EdgeSqueeze(Edge* e, Vert* vSurvivor) {
    // EdgeSqueeze is implemented using the GlueVert operator
    // it performs the same action, but it is known before hand that v1 and v2 have a common edge
    // this means that search for common edge can be omitted
    // that search is performed in O(m*n), where m and n are the number of edges aroud v1 and v2
    // the performance could be improved slightly, but m, n are rarely large, and in quad mesh, manifold scenario
    // m, n = 4, which means worst case - 16 iterations, which isn't much 

    GlueVert(vSurvivor, e->Other(vSurvivor));
    return;
}

} // namespace Core
} // namespace Aoba
