#ifndef AOBA_CORE_MESH_MESH_HPP
#define AOBA_CORE_MESH_MESH_HPP

#include "../../Math/Matrix/Matrix4.hpp"
#include "../EulerOps.hpp"

#include <vector>
#include <functional>

namespace Aoba {
namespace Core {

class Vert;
class Edge;
class Face;

class Mesh {
    friend void EdgeSplit(Edge*, Vert*, Edge*, Vert*);
    friend void KillEdge(Edge*);
    friend void KillFace(Face*);
    friend void KillMesh(Mesh*);
    friend void KillVert(Vert*);
    friend void MakeEdge(Vert*, Vert*, Edge*);
    friend void MakeEdgeVert(Vert*, Edge*, Vert*);
    friend void MakeFace(Loop*, Face*);
    friend void MakeVert(Mesh*, Vert*);
    friend void GlueVert(Vert*, Vert*);
    friend void ManifoldMakeEdge(Vert*, Vert*, Face*, Edge*, Face*);
    friend void DissolveEdge(Edge*, Face*);
    friend void JoinMesh(Mesh*, Mesh*);

  private:
    Vert* verts; // List of all verts in the mesh.
    Edge* edges; // List of all edges in the mesh.
    Face* faces; // list of all faces in the mesh.
                 // List of loops is omitted, because loops can be easily accessed using faces.
  public:
    /// <summary>
    /// Constructor, initializes empty lists for verts, edges and faces.
    /// </summary>
    Mesh();

    /// <summary>
    /// Checks wether the mesh is in a valid state.
    /// </summary>
    /// <returns>True if the mesh is in a valid state, otherwise False.</returns>
    bool IsValid() const;

    /// <summary>
    /// Transform all mesh elements using a 4x4 transformation matrix.
    /// </summary>
    /// <param name="mat">4x4 transformation matrix to use.</param>k
    void Transform(Math::Mat4 mat);

    /// <summary>
    /// List of all Verts inside the mesh. Do not use this list to add new verts to the mesh, use EulerOps instead.
    /// </summary>
    /// <returns>List containing references to all Verts inside the mesh. </returns>
    const std::vector<Vert*> Verts() const;

    /// <summary>
    /// List of all Edges inside the mesh. Do not use this list to add new Edges to the mesh, use EulerOps instead.
    /// </summary>
    /// <returns>List containing references to all Edges inside the mesh. </returns>
    const std::vector<Edge*> Edges() const;

    /// <summary>
    /// List of all Faces inside the mesh. Do not use this list to add new Faces to the mesh, use EulerOps instead.
    /// </summary>
    /// <returns>List containing references to all Faces inside the mesh. </returns>
    const std::vector<Face*> Faces() const;

    /// <summary>
    /// List of verts iun the mesh which fulfill the criteria given by the filtering function.
    /// </summary>
    /// <param name="func">Filtering function</param>
    /// <returns>Filtered verts</returns>
    const std::vector<Vert*> Verts(std::function<bool(const Vert* const)> func) const;

    /// <summary>
    /// List of edges iun the mesh which fulfill the criteria given by the filtering function.
    /// </summary>
    /// <param name="func">Filtering function</param>
    /// <returns>Filtered edges</returns>
    const std::vector<Edge*> Edges(std::function<bool(const Edge* const)> func) const;

    /// <summary>
    /// List of faces iun the mesh which fulfill the criteria given by the filtering function.
    /// </summary>
    /// <param name="func">Filtering function</param>
    /// <returns>Filtered faces</returns>
    const std::vector<Face*> Faces(std::function<bool(const Face* const)> func) const;
};

} // namespace Core
} // namespace Aoba

#endif