#ifndef AOBA_CORE_MESH_LOOP_HPP
#define AOBA_CORE_MESH_LOOP_HPP

#include "../EulerOps.hpp"

#include <cstdint>

namespace Aoba {
namespace Core {

class Vert;
class Edge;
class Face;
class Mesh;

class Loop {
    friend void EdgeSplit(Edge*, Vert*, Edge*, Vert*);
    friend void KillFace(Face*);
    friend void MakeFace(Loop*, Face*);
    friend void MakeLoop(std::vector<Edge*>, std::vector<Vert*>, Loop*);
    friend void GlueVert(Vert*, Vert*);
    friend void ManifoldMakeEdge(Vert*, Vert*, Face*, Edge*, Face*);
    friend void GlueFace(Face*, Edge*, Face*, Edge*);

    friend class Edge;
    friend class Face;
    friend class Vert;

  public:
    std::size_t index;   // index of this vert, not updated automatically, used for tools
    int32_t flags;       // flags available for use in other tools
    int32_t flagsIntern; // flags for use in internal tools and operators
  private:
    Vert* v;     // First vert of the edge which this loop uses. Specifies edge orientation.
    Edge* e;     // Edge which this loop uses. Uses verts v, next->v
    Face* f;     // Face which is bound by this loop.
    Loop* eNext; // List of loops around edge e.
    Loop* ePrev; // List of loops around edge e.
    Loop* fNext; // List of loops which form the boundary of face f.
    Loop* fPrev; // List of loops which form the boundary of face f.
    Mesh* m;     // Pointer to parent mesh. Could potentialy be ommited but might pose a safety issue.

  public:
    Loop();
    Vert* LoopVert() const;
    Edge* LoopEdge() const;
    Face* LoopFace() const;
};

} // namespace Core
} // namespace Aoba
#endif