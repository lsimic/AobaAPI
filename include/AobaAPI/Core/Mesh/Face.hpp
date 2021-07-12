#ifndef AOBA_CORE_MESH_FACE_HPP
#define AOBA_CORE_MESH_FACE_HPP

#include "../../Math/Vector/Vector3.hpp"
#include "../EulerOps.hpp"

#include <cstdint>
#include <vector>
#include <functional>

namespace Aoba {
namespace Core {

class Loop;
class Mesh;
class Edge;
class Vert;

class Face {
    friend class Mesh;

    friend void KillFace(Mesh*, Face*);
    friend void KillMesh(Mesh*);
    friend void MakeFace(Mesh*, Loop*, Face*);
    friend void ManifoldMakeEdge(Mesh*, Vert*, Vert*, Face*, Edge*, Face*);
    friend void GlueVert(Mesh*, Vert*, Vert*);
    friend void DissolveEdge(Mesh*, Edge*, Face*);
    friend void JoinMesh(Mesh*, Mesh*);

  private:
    Loop* l;     // First loop in a list of loops which form the face boundary. use fNext/fPrev for traversal
    Mesh* m;     // Pointer to parent mesh. Could potentialy be ommited but might pose a safety issue.
  public:
    std::size_t index;   // index of this vert, not updated automatically, used for tools
    int32_t flags;       // flags available for use in other tools
    int32_t flagsIntern; // flags for use in internal tools and operators
    Math::Vec3 no;       // Face normal
    short materialIdx;   // Face material index

    Face();

    /// <summary>
    /// Calculate the area of the face.
    /// </summary>
    /// <returns>Area of the face.</returns>
    float CalcArea() const;

    /// <summary>
    /// Calculate the center of the face as an average value of it's verts.
    /// </summary>
    /// <returns>Center of face.</returns>
    Math::Vec3 CalcCenterAverage() const;

    /// <summary>
    /// Calculate the sum of lengths of all edges which form this face.
    /// </summary>
    /// <returns>Perimiter of the face.</returns>
    float CalcPerimiter() const;

    /// <summary>
    /// Flip the normal of this face by changing the winding of it's edges/loops.
    /// </summary>
    void NormalFlip();

    /// <summary>
    /// Update the face's normal.
    /// </summary>
    void NormalUpdate();

    /// <summary>
    /// List of all edges of the face. Do not use this list to change edges, use EulerOps instead.
    /// </summary>
    /// <returns>Unordered List containing references to all edges of this face.</returns>
    const std::vector<Edge*> Edges() const;

    /// <summary>
    /// List of all Verts of the face. Do not use this list to change verts, use EulerOps instead.
    /// </summary>
    /// <returns>Unordered List containing references to all verts of this face.</returns>
    const std::vector<Vert*> Verts() const;

    /// <summary>
    /// List of all loops of the face. Do not use this list to modify the face, use EulerOps instead.
    /// </summary>
    /// <returns>Ordered list of loops using this face.</returns>
    const std::vector<Loop*> Loops() const;

    /// <summary>
    /// List of edges which form this face and fulfill the criteria given by the filtering function. 
    /// </summary>
    /// <param name="func">Filtering function</param>
    /// <returns>Filtered edges</returns>
    const std::vector<Edge*> Edges(std::function<bool(const Edge* const)> func) const;

    /// <summary>
    /// List of verts which form this face and fulfill the criteria given by the filtering function. 
    /// </summary>
    /// <param name="func">Filtering function</param>
    /// <returns>Filtered verts</returns>
    const std::vector<Vert*> Verts(std::function<bool(const Vert* const)> func) const;

    /// <summary>
    /// List of loops which form this face and fulfill the criteria given by the filtering function. 
    /// </summary>
    /// <param name="func">Filtering function</param>
    /// <returns>Filtered loops</returns>
    const std::vector<Loop*> Loops(std::function<bool(const Loop* const)> func) const;
};

} // namespace Core
} // namespace Aoba

#endif