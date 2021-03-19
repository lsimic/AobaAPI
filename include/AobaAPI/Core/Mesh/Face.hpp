#ifndef AOBA_CORE_MESH_FACE_HPP
#define AOBA_CORE_MESH_FACE_HPP

#include "../../Math/Vector/Vector3.hpp"
#include "../EulerOps.hpp"

#include <vector>

namespace Aoba {
namespace Core {

class Loop;
class Mesh;
class Edge;
class Vert;

class FaceLoopList {
    friend void MakeFace(std::vector<Edge*>, Face*);
    friend void MakeFace(std::vector<Loop*>, Face*);

    friend class Face;

  private:
    FaceLoopList* next; // List of loops which form the face boundary.
    FaceLoopList* prev; // List of loops which form the face boundary.
    Loop* first;        // Any Element of the loop. Use first->fNext/fPrev for traversal.

  public:
    FaceLoopList();

    /// <summary>
    /// All loops in this Loop. Ordered.
    /// </summary>
    /// <returns>TODO</returns>
    const std::vector<Loop*> Loops() const;
};

class Face {
    friend void MakeFace(std::vector<Edge*>, Face*);
    friend void MakeFace(std::vector<Loop*>, Face*);

    friend class Mesh;

  private:
    FaceLoopList* loops; // List of loops which form the face boundary
    Mesh* m;             // Pointer to parent mesh. Could potentialy be ommited but might pose a safety issue.
    Face* mNext;         // list of all faces in the mesh
    Face* mPrev;         // List of all faces in the mesh
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
    /// <returns>TODO </returns>
    const std::vector<FaceLoopList*> LoopLists() const;
};

} // namespace Core
} // namespace Aoba

#endif