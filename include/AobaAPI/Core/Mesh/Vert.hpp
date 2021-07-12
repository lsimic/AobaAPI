#ifndef AOBA_CORE_MESH_VERT_HPP
#define AOBA_CORE_MESH_VERT_HPP

#include "../../Math/Vector/Vector3.hpp"
#include "../EulerOps.hpp"

#include <cstdint>
#include <vector>
#include <functional>

namespace Aoba {
namespace Core {

class Edge;
class Mesh;
class Face;

class Vert {
    friend class Mesh;

    friend void EdgeSplit(Mesh*, Edge*, Vert*, Edge*, Vert*);
    friend void KillEdge(Mesh*, Edge*);
    friend void KillMesh(Mesh*);
    friend void KillVert(Mesh*, Vert*);
    friend void MakeEdge(Mesh*, Vert*, Vert*, Edge*);
    friend void MakeEdgeVert(Mesh*, Vert*, Edge*, Vert*);
    friend void MakeVert(Mesh*, Vert*);
    friend void GlueVert(Mesh*, Vert*, Vert*);
    friend void JoinMesh(Mesh*, Mesh*);

  public:
    std::size_t index;   // index of this vert, not updated automatically, used for tools
    int32_t flags;       // flags available for use in other tools
    int32_t flagsIntern; // flags for use in internal tools and operators
    Math::Vec3 co;       // X,Y,Z vertex coordinates
    Math::Vec3 no;       // X,Y,Z vertex normals
  private:
    Edge* e;     // List of edges using this vert. Use e->v1/v2 Next/Prev for traversal.
    Mesh* m;     // Pointer to parent mesh. Could potentialy be ommited but might pose a safety issue.
  public:
    Vert();

    /// <summary>
    /// Check wether the vert is boundary. Vert is boundary if it is connected to a boundary edge.
    /// Isolated verts are not considered boundary verts.
    /// </summary>
    /// <returns>True if boundary vert, otherwise false.</returns>
    bool IsBoundary() const;

    /// <summary>
    /// Check wether the vert is manifold. Vert is not manifold if it belongs to multiple-face edges or wire edges,
    /// borders non-adjecent faces or is an isolated vert.
    /// </summary>
    /// <remarks>
    /// This is sasauge.
    /// </remarks>
    /// <returns>True if vert is manifold, otherwise false.</returns>
    bool IsManifold() const;

    /// <summary>
    /// Check wether the vert is wire. Vert is wire if it is adjecent to a wire edge.
    /// Isolated verts are not considered wire verts.
    /// </summary>
    /// <returns>True if vert is wire, otherwise false.</returns>
    bool IsWire() const;

    /// <summary>
    /// List of all Edges that use this vert. Do not use this list to add new Edges, use EulerOps instead.
    /// </summary>
    /// <returns>List containing references to all Edges using this vert.</returns>
    const std::vector<Edge*> Edges() const;

    /// <summary>
    /// List of all faces that use this vert. Do not use this list to add new faces, use EulerOps instead.
    /// </summary>
    /// <returns>List containing references to all faces using this vert.</returns>
    const std::vector<Face*> Faces() const;

    /// <summary>
    /// List of all loops which start in this vert. Do not use this list to add new loops, use EulerOps instead.
    /// </summary>
    /// <returns>Loops using this vert</returns>
    const std::vector<Loop*> Loops() const;

    /// <summary>
    /// List of edges which are adjacent to this vert and fulfill the criteria given by the filtering function.
    /// </summary>
    /// <param name="func">Filtering function</param>
    /// <returns>Filtered edges</returns>
    const std::vector<Edge*> Edges(std::function<bool(const Edge* const)> func) const;

    /// <summary>
    /// List of faces which are adjacent to this vert and fulfill the criteria given by the filtering function.
    /// </summary>
    /// <param name="func">Filtering function</param>
    /// <returns>Filtered faces</returns>
    const std::vector<Face*> Faces(std::function<bool(const Face* const)> func) const;

    
    /// <summary>
    /// List of loops which are adjacent to this vert and fulfill the criteria given by the filtering function.
    /// </summary>
    /// <param name="func">Filtering function</param>
    /// <returns>Filtered loops</returns>
    const std::vector<Loop*> Loops(std::function<bool(const Loop* const)> func) const;

};
} // namespace Core
} // namespace Aoba

#endif