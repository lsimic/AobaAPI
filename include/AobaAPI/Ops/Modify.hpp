#ifndef AOBA_OPS_MODIFY_HPP
#define AOBA_OPS_MODIFY_HPP

#include "AobaAPI/Core.hpp"
#include "AobaAPI/Math.hpp"

#include <vector>

namespace Aoba {
namespace Ops {

enum class DeleteMode { All, FacesOnly, FacesAndEdges };

/// <summary>
/// Deletes the given mesh elements according to the specified delete mode.
/// </summary>
/// <param name="m">Mesh on which to operate on</param>
/// <param name="verts">Verts to delete</param>
/// <param name="edges">Edges to delete</param>
/// <param name="faces">Faces to delete</param>
/// <param name="mode">Delete mode which specifies how the delete operation will behave</param>
void Delete(Core::Mesh* m, const std::vector<Core::Vert*>& verts, const std::vector<Core::Edge*>& edges,
    const std::vector<Core::Face*>& faces, DeleteMode mode);

class DuplicateResult {
  public:
    std::vector<Core::Vert*> verts; // new verts
    std::vector<Core::Face*> faces; // new faces
    std::vector<Core::Edge*> edges; // new edges
};

/// <summary>
/// Duplicate the given mesh elements, keeping them in the original mesh.
/// </summary>
/// <param name="m">Mesh on which to operate on</param>
/// <param name="verts">Verts to duplicate</param>
/// <param name="edges">Edges to duplicate</param>
/// <param name="faces">Faces to duplicate</param>
/// <returns>Duplicated elements</returns>
const DuplicateResult Duplicate(Core::Mesh* m, const std::vector<Core::Vert*>& verts,
    const std::vector<Core::Edge*>& edges, const std::vector<Core::Face*>& faces);

class ExtrudeEdgesResult {
  public:
    std::vector<Core::Vert*> verts;           // new verts
    std::vector<Core::Edge*> horizontalEdges; // edges connecting new verts
    std::vector<Core::Edge*> verticalEdges;   // edges connecting new and original verts
    std::vector<Core::Face*> faces;           // new faces
};

/// <summary>
/// Extrude edges into individual faces.
/// </summary>
/// <param name="m">Mesh on which to operate on</param>
/// <param name="edges">Edges to extrude</param>
/// <returns>new verts, edges and faces</returns>
const ExtrudeEdgesResult ExtrudeEdges(Core::Mesh* m, const std::vector<Core::Edge*>& edges);

class ExtrudeFacesResult {
  public:
    std::vector<Core::Vert*> verts;           // new verts
    std::vector<Core::Face*> horizontalFaces; // faces formed by new horizontal edges
    std::vector<Core::Edge*> horizontalEdges; // edges connecting new verts
    std::vector<Core::Face*> verticalFaces;   // edges connecting new and original verts
    std::vector<Core::Edge*> verticalEdges;   // faces formed between new and original edges
};

/// <summary>
/// Extrude individual faces.
/// </summary>
/// <param name="m">Mesh on which to operate on</param>
/// <param name="faces">Faces to extrude</param>
/// <param name="keepOrig">wether to keep or delete original faces</param>
/// <returns>New verts, edges and faces</returns>
const ExtrudeFacesResult ExtrudeFaces(Core::Mesh* m, const std::vector<Core::Face*>& faces, bool keepOrig);

class ExtrudeFaceRegionResult {
  public:
    // TODO: perhaps some things could be kept "in place" here
    std::vector<Core::Vert*> verts;           // new verts
    std::vector<Core::Face*> horizontalFaces; // faces formed by new horizontal edges
    std::vector<Core::Edge*> horizontalEdges; // edges connecting new verts
    std::vector<Core::Face*> verticalFaces;   // edges connecting new and original verts
    std::vector<Core::Edge*> verticalEdges;   // faces formed between new and original edges
};

/// <summary>
/// Extrude face region. Only the boundary edges are extruded into faces
/// </summary>
/// <param name="m">Mesh on which to operate on</param>
/// <param name="faces">Faces to extrude</param>
/// <param name="keepOrig">wether to keep or delete original faces</param>
/// <returns>new edges, verts and faces</returns>
const ExtrudeFaceRegionResult ExtrudeFaceRegion(Core::Mesh* m, const std::vector<Core::Face*>& faces, bool keepOrig);

class ExtrudeVertsResult {
  public:
    std::vector<Core::Vert*> verts; // new verts
    std::vector<Core::Edge*> edges; // new edges
};

/// <summary>
/// Extrude individual verts into wire edges
/// </summary>
/// <param name="m">Mesh on which to operate on</param>
/// <param name="verts">Verts to extrude</param>
/// <returns>New edges and verts</returns>
const ExtrudeVertsResult ExtrudeVerts(Core::Mesh* m, const std::vector<Core::Vert*>& verts);

/// <summary>
/// Make a new face bound by the given edges
/// </summary>
/// <param name="m">Mesh on which to operate on</param>
/// <param name="edges">Continous loop of edges which form the face boundary</param>
/// <returns>The new face</returns>
/// <exception cref="std::invalid_argument">
/// Thrown if the edges do not form a valid loop/face boundary
/// </exception>
Core::Face* MakeFace(Core::Mesh* m, std::vector<Core::Edge*> edges);

class InsetFacesResult {
  public:
    std::vector<Core::Vert*> verts;         // new verts
    std::vector<Core::Face*> centerFaces;   // face found in the center
    std::vector<Core::Edge*> centerEdges;   // edges forming the boundary of the center face
    std::vector<Core::Face*> boundaryFaces; // faces found around the original face loop
    std::vector<Core::Edge*> boundaryEdges; // edges connecting the original and new verts
};

/// <summary>
/// Inset individual faces
/// </summary>
/// <param name="m">Mesh on which to operate on</param>
/// <param name="faces">Faces to inset</param>
/// <param name="distance">Thickness of the new face-loop around the face boundary</param>
/// <returns>New verts, edges and faces</returns>
const InsetFacesResult InsetFaces(Core::Mesh* m, const std::vector<Core::Face*>& faces, float distance);

class MirrorResult {
  public:
    std::vector<Core::Vert*> verts;       // new verts
    std::vector<Core::Face*> faces;       // new faces
    std::vector<Core::Edge*> edges;       // new edges
    std::vector<Core::Vert*> mergedVerts; // verts merged due to mergeDist
};

/// <summary>
/// Mirror the input elements around the plane specified by the given axis and center.
/// If a vert lies on or near the mirror plane, if specified, it will be glued onto the original vert.
/// </summary>
/// <param name="m">Mesh on which to operate on</param>
/// <param name="verts">Verts to mirror</param>
/// <param name="edges">Edges to mirror</param>
/// <param name="faces">Faces to mirror</param>
/// <param name="axis">Mirror axis, specifies the mirror plane normal</param>
/// <param name="center">Center, specifies the mirror plane position</param>
/// <param name="merge">Wether to merge verts near the mirror plane</param>
/// <param name="mergeDist">Distance of a vert from mirror plane below which it will be merged</param>
/// <returns>New verts, edges, faces and merged verts</returns>
const MirrorResult Mirror(Core::Mesh* m, const std::vector<Core::Vert*>& verts, const std::vector<Core::Edge*>& edges,
    const std::vector<Core::Face*>& faces, const Math::Vec3& axis, const Math::Vec3& center, bool merge,
    float mergeDist);

/// <summary>
/// Recalculate face normal for given faces
/// </summary>
/// <param name="m">Mesh on which to operate on</param>
/// <param name="faces">Faces to operate on</param>
void RecalculateFaceNormals(Core::Mesh* m, const std::vector<Core::Face*>& faces);

/// <summary>
/// Reverse ordering of the face loop, flipping its orientation
/// </summary>
/// <param name="m">Mesh on which to operate on</param>
/// <param name="faces">Faces to operate on</param>
void ReverseFaceOrder(Core::Mesh* m, const std::vector<Core::Face*>& faces);

class SplitEdgesResult {
  public:
    std::vector<Core::Vert*> verts; // new verts
    std::vector<Core::Edge*> edges; // new edges
};

/// <summary>
/// Split the input edges by adding 2-valence verts. The ratios of resulting edge lengths must be specified.
/// ratios.size() = cuts+1
/// sum(ratios) = 1
/// </summary>
/// <param name="m">Mesh on which to operate on</param>
/// <param name="edges">Edges to split</param>
/// <param name="cuts">Number of rations</param>
/// <param name="ratios">Edge ratios, sum of all must be equal to 1, size must be cuts + 1</param>
/// <returns>new edges and verts</returns>
/// <exception cref="std::invalid_argument">
/// Thrown if ratios.size() != cuts +1
/// thrown if sum of all ratios != 1.
/// <exception>
const SplitEdgesResult SplitEdges(
    Core::Mesh* m, const std::vector<Core::Edge*>& edges, unsigned cuts, const std::vector<float>& ratios);

class TriangulateFacesResult {
  public:
    std::vector<Core::Edge*> edges; // new edges
    std::vector<Core::Face*> faces; // new faces
};

/// <summary>
/// Triangulate input faces using a simple fan triangulation.
/// </summary>
/// <param name="m">Mesh on which to operate on</param>
/// <param name="faces">Faces to triangulate</param>
/// <returns>new edges and faces</returns>
const TriangulateFacesResult TriangulateFaces(Core::Mesh* m, const std::vector<Core::Face*>& faces);

class SubdivideResult {
  public:
    std::vector<Core::Edge*> edges;     // new edges connecting edgeVerts to faceVerts and split origuinal edges
    std::vector<Core::Vert*> edgeVerts; // new verts made by splitting edges
    std::vector<Core::Vert*> faceVerts; // new verts created at face centers
    std::vector<Core::Face*> faces;     // new faces
};

/// <summary>
/// Subdivides input geometry one step, without applying any smoothing.
/// Only the input faces will be subdivided, even if their entire boundary is found in input edges. 
/// </summary>
/// <param name="m">Mesh on which to operate on</param>
/// <param name="edges">wire/isolated edges to subdivide</param>
/// <param name="faces">Faces to subdivide</param>
/// <returns>newly created edges, verts and faces</returns>
const SubdivideResult SubdivideSimple(
    Core::Mesh* m, const std::vector<Core::Edge*>& edges, const std::vector<Core::Face*>& faces);


/// <summary>
/// Subdivides input geometry one step, applying the catmull-clark smoothing argorithm. 
/// Only the input faces will be subdivided, even if their entire boundary is found in input edges.
/// </summary>
/// <param name="m">Mesh on which to operate on</param>
/// <param name="edges">wire/isolated edges to subdivide</param>
/// <param name="faces">Faces to subdivide</param>
/// <returns>newly created edges, verts and faces</returns>
const SubdivideResult SubdivideSmooth(
    Core::Mesh* m, const std::vector<Core::Edge*>& edges, const std::vector<Core::Face*>& faces);

} // namespace Ops
} // namespace Aoba

#endif