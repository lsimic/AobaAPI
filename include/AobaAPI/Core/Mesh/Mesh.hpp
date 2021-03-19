#ifndef AOBA_CORE_MESH_MESH_HPP
#define AOBA_CORE_MESH_MESH_HPP

#include "../../Math/Matrix/Matrix4.hpp"
#include "../EulerOps.hpp"

#include <vector>

namespace Aoba {
namespace Core {

class Vert;
class Edge;
class Face;

class Mesh {
    friend void MakeEdge(Vert*, Vert*, Edge*);
    friend void MakeEdgeVert(Vert*, Edge*, Vert*);
    friend void MakeFace(std::vector<Edge*>, Face*);
    friend void MakeFace(std::vector<Loop*>, Face*);
    friend void MakeVert(Mesh*, Vert*);

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
    /// <param name="">4x4 transformation matrix to use.</param>k
    void Transform(Math::Mat4);

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
};

} // namespace Core
} // namespace Aoba

#endif