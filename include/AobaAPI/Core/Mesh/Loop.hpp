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
    friend class Edge;
    friend class Face;
    friend class Vert;
    friend class Mesh;

    friend void EdgeSplit(Mesh*, Edge*, Vert*, Edge*, Vert*);
    friend void KillFace(Mesh*, Face*);
    friend void MakeFace(Mesh*, Loop*, Face*);
    friend void MakeLoop(Mesh*, std::vector<Edge*>, std::vector<Vert*>, Loop*);
    friend void MakeLoop(Mesh*, std::vector<Edge*>, std::vector<Vert*>, std::vector<Loop*>);
    friend void GlueVert(Mesh*, Vert*, Vert*);
    friend void ManifoldMakeEdge(Mesh*, Vert*, Vert*, Face*, Edge*, Face*);
    friend void GlueFace(Mesh*, Face*, Edge*, Face*, Edge*);
    friend void DissolveEdge(Mesh*, Edge*, Face*);
    friend void JoinMesh(Mesh*, Mesh*);

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

    /// <summary>
    /// Vert which defines the direction of this loop.
    /// </summary>
    /// <returns>Vert of the loop</returns>
    Vert* LoopVert() const;

    /// <summary>
    /// Edge which forms this loop.
    /// </summary>
    /// <returns>Edge of the loop</returns>
    Edge* LoopEdge() const;

    /// <summary>
    /// Face which this loop forms.
    /// </summary>
    /// <returns>Face of the loop</returns>
    Face* LoopFace() const;
};

} // namespace Core
} // namespace Aoba
#endif