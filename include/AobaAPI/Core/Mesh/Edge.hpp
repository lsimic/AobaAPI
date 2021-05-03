#ifndef AOBA_CORE_MESH_EDGE_HPP
#define AOBA_CORE_MESH_EDGE_HPP

#include "../EulerOps.hpp"
#include "../../Math/Vector/Vector3.hpp"

#include <cstdint>
#include <vector>
#include <functional>

namespace Aoba {
namespace Core {

class Vert;
class Loop;
class Mesh;
class Face;

class Edge {
    friend class Vert;
    friend class Mesh;
    friend class Face;

    friend void EdgeSplit(Edge*, Vert*, Edge*, Vert*);
    friend void KillEdge(Edge*);
    friend void KillFace(Face*);
    friend void KillMesh(Mesh*);
    friend void MakeEdge(Vert*, Vert*, Edge*);
    friend void MakeEdgeVert(Vert*, Edge*, Vert*);
    friend void MakeFace(std::vector<Edge*>, Face*);
    friend void MakeLoop(std::vector<Edge*>, std::vector<Vert*>, Loop*);
    friend void GlueVert(Vert*, Vert*); 
    friend void ManifoldMakeEdge(Vert*, Vert*, Face*, Edge*, Face*);
    friend void GlueEdge(Edge*, Edge*);

  public:
    std::size_t index;   // index of this vert, not updated automatically, used for tools
    int32_t flags;       // flags available for use in other tools
    int32_t flagsIntern; // flags for use in internal tools and operators
  private:
    Vert* v1;     // Unordered verts of this edge.
    Vert* v2;     // Unordered verts of this edge.
    Loop* l;      // List of loops using this edge. Use l->eNext/ePrev for traversal.
    Edge* v1Next; // List of edges around v1
    Edge* v1Prev; // List of edges around v1
    Edge* v2Next; // List of edges around v2
    Edge* v2Prev; // List of edges around v2
    Mesh* m;      // Pointer to parent mesh. Could potentialy be ommited but might pose a safety issue.
    Edge* mNext;  // list of all edges in the mesh
    Edge* mPrev;  // List of all edges in the mesh

    /// <summary>
    /// Get the next vert in the list of edges around vert v
    /// </summary>
    /// <param name="v"></param>
    /// <returns></returns>
    Edge* Next(const Vert* v) const;

    /// <summary>
    /// Get the previous edge in the list of edges around vert v
    /// </summary>
    /// <param name="v"></param>
    /// <returns></returns>
    Edge* Prev(const Vert* v) const;

    /// <summary>
    /// Calculate the local face normal (adjecent triangle or quad)
    /// </summary>
    /// <param name="loop">Loop of this edge to use for calculation.</param>
    /// <returns></returns>
    Math::Vec3 CalcLocalNormal(Loop* loop) const;

  public:
    Edge();

    /// <summary>
    /// Calculate angle between two faces.
    /// </summary>
    /// <returns>Angle between faces.</returns>
    float CalcFaceAngle() const;

    /// <summary>
    /// Calculate angle between two faces, negative for concave join.
    /// </summary>
    /// <returns>Angle between faces.</returns>
    float CalcFaceAngleSigned() const;

    /// <summary>
    /// Euclidean distance between the verts of this edge.
    /// </summary>
    /// <returns>Length of the edge.</returns>
    float CalcLength() const;

    /// <summary>
    /// Find the other vert of this edge.
    /// </summary>
    /// <param name="v">Any vert of this edge.</param>
    /// <returns>Other vert of this edge.</returns>
    Vert* Other(const Vert* v) const;

    /// <summary>
    /// Check wether the edge is boundary face. Edge is boundary if it is used by only one face, and is used only once
    /// in the loop
    /// </summary>
    /// <returns></returns>
    bool IsBoundary() const;

    /// <summary>
    /// Check wether the edge is contigous. Edge is not contigous if it has two faces pointing in separate directions.
    /// Wire edges and boundary edges are always contigous. Edges used three or more times are never contigous.
    /// </summary>
    /// <returns>True if edge is contigous, otherwise false.</returns>
    bool IsContigous() const;

    /// <summary>
    /// Check wether the edge is manifold. Edge is not manifold if it has three or more adjecent loops, or is a wire
    /// edge, or has adjecent faces pointing in separate directions.
    /// </summary>
    /// <returns>True if edge is manifold, otherwise false.</returns>
    bool IsManifold() const;

    /// <summary>
    /// Check wether the edge is a wire edge. Edge is a wire edge if it does not have any adjecent face.
    /// </summary>
    /// <returns>True if edge is wire, otherwise false.</returns>
    bool IsWire() const;

    /// <summary>
    /// List of all Faces that use this edge. Do not use this list to add new faces, use EulerOps instead.
    /// </summary>
    /// <returns>Unordered List containing references to all Faces which use this edge.</returns>
    const std::vector<Face*> Faces() const;

    /// <summary>
    /// 
    /// </summary>
    /// <param name=""></param>
    /// <returns></returns>
    const std::vector<Face*> Faces(std::function<bool(const Face* const)>) const;

    /// <summary>
    /// List of all Loops that use this edge. Do not use this list to add new Loops, use EulerOps instead.
    /// </summary>
    /// <returns>Unordered List containing references to all Loops which use this edge.</returns>
    const std::vector<Loop*> Loops() const;

    /// <summary>
    /// 
    /// </summary>
    /// <param name=""></param>
    /// <returns></returns>
    const std::vector<Loop*> Loops(std::function<bool(const Loop* const)>) const;

    /// <summary>
    /// List of all Verts of the edge. Do not use this list to change verts, use EulerOps instead.
    /// </summary>
    /// <returns>Unordered List containing references to all verts of this edge.</returns>
    const std::vector<Vert*> Verts() const;

    /// <summary>
    /// 
    /// </summary>
    /// <param name=""></param>
    /// <returns></returns>
    const std::vector<Vert*> Verts(std::function<bool(const Vert* const)>) const;

    /// <summary>
    /// V1 of this edge. Do not use this to change the verts, use EulerOps instead.
    /// </summary>
    /// <returns></returns>
    Vert* V1() const;
    
    /// <summary>
    /// V1 of this edge. Do not use this to change the verts, use EulerOps instead.
    /// </summary>
    /// <returns></returns>
    Vert* V2() const;
};
} // namespace Core
} // namespace Aoba

#endif