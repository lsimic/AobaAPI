#include "AobaAPI/Core/EulerOps.hpp"
#include "AobaAPI/Core/Mesh.hpp"

namespace Aoba {
namespace Core {

void GlueEdge(Edge* e1, Edge* e2) {
    // Glue edge is implemented using the gluevert operator
    // it performs the same actions as the glue vert operator, but it is known that the references to e2 will be
    // replaced with references to e1

    // since other check must be performed, some speedup might be achieved by implementing this operator separately

    // check wether edge e1 and edge e2 have any shared verts
    // if they have a shared vert, the way in which they will be glued is fixed 
    // and only one glueVert operation is required
    if(e1->v1 == e2->v1) {
        GlueVert(e1->v2, e2->v2);
        return;
    }
    if(e1->v1 == e2->v2) {
        GlueVert(e1->v2, e2->v1);
        return;
    }
    if(e1->v2 == e2->v1) {
        GlueVert(e1->v1, e2->v2);
        return;
    }
    if(e1->v2 == e2->v2) {
        GlueVert(e1->v1, e2->v1);
        return;
    }

    GlueVert(e1->v1, e2->v1);
    GlueVert(e1->v2, e2->v2);
    return;

    // verts of v1 are always kept, in order to always keep the edges of v1 alive
}

} // namespace Core
} // namespace Aoba
