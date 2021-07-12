#include "AobaAPI/Core/EulerOps.hpp"
#include "AobaAPI/Core/Mesh.hpp"

namespace Aoba {
namespace Core {

void KillVert(Mesh* m, Vert* v) {
    // Kill all edges (and faces) using this edge
    std::vector<Edge*> vertEdges = v->Edges();
    if(vertEdges.size() > 0) {
        for(auto it = vertEdges.begin(); it != vertEdges.end(); ++it) {
            KillEdge(m, (*it));
        }
    }

    m->vertPool.Free(v);
}

} // namespace Core
} // namespace Aoba