#ifndef AOBA_CORE_EULEROPS_HPP
#define AOBA_CORE_EULEROPS_HPP

#include <vector>

namespace Aoba {
namespace Core {

class Mesh;
class Vert;
class Edge;
class Face;
class Loop;

/// <summary>
/// Creates a new Vert newv in an existing mesh m.
/// Adds the Vert newv to the list of verts in the mesh with correct pointers
/// </summary>
/// <param name="m"></param>
/// <param name="newv"></param>
void MakeVert(Mesh* m, Vert* newv);

/// <summary>
/// Creates a single Face newf, which has the single Loop newl, consisting of the existing single Vert v. The operator
/// can be thought of as creating a spherical surface which touches upon Vert v.
/// </summary>
/// <param name="v"></param>
/// <param name="newf"></param>
/// <param name="newl"></param>
void MakeFaceLoop(Vert* v, Face* newf, Loop* newl);

/// <summary>
/// Deletes the Vert v and any Edges which touch upon it, deleting Loops and Faces as necessary.
/// KillVert will not delete a Vert when deletion of the Vert(and/or Edges incident to it) would cause the creation of a
///  non-manifold Face. In this case an exception will occur and no action will be performed.
/// This is NOT equivalent to calling ~Vert()!
/// </summary>
/// <param name="v"></param>
/// <exception cref="System.OverflowException">
/// Thrown when the deletion of the Vert would cause the creation of a non-manifold Face.
/// </exception>
void KillVert(Vert* v);

/// <summary>
/// Deletes the Edge e. KillEdge will not delete an Edge when deletion of the Edge would cause the creation of a
/// non-manifold Face. This condition always occurs with non-manifold Edges where the Edge is used three or more times
/// by the same Face. In this case an error will result and no action will be performed.
/// This is NOT equivalent to calling ~Edge()!
/// </summary>
/// <param name="e"></param>
/// <param name="v"></param>
/// <param name="fSurvivor"></param>
/// <param name="newl"></param>
/// <exception cref="System.OverflowException">
/// Thrown when the deletion of the Edge would cause the creation of a non-manifold Face.
/// </exception>
void KillEdge(Edge* e, Vert* v, Face* fSurvivor, Loop* newl);

/// <summary>
/// Merges the vertices of v1 and v2 together, preserving the adjecencies of elements, the Vert of v1 is the
/// surviving Vert. The Vert of v2 is deleted.
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
void GlueVert(Vert* v1, Vert* v2);

/// <summary>
/// Merges the Edge e1 together with the Edge e2 preserving the adhecencies of elements. The Edge e1 is the
/// surviving Edge. The Edge e2 and any of its vertices which are not shared with the Edge e1 are deleted.
/// </summary>
/// <param name="e1"></param>
/// <param name="e2"></param>
void GlueEdge(Edge* e1, Edge* e2);

/// <summary>
/// Merges the single loop faces f1 and f2 together, preserving the adjecencies of elements.e1 and e22 specify exactly
/// how and in what direction the two loop boundaries match up. The loops are glued togerher with the edges in
/// opposite orientation. The face f1 is the surviving face. The face f2 and its loop, edges and vertices not shared
/// with the face f1 are deleted. The faces specified by both f1 and f2 must each have one only loop with the same
/// number of edges with self loop, isthmus and strut edges in an identical order in both loops or an error will result
///  and no action will be performed. Note that if the faces f1 and f2 share any edges or two or more vertices the
/// acceptable orientations for glue operations may already be fixed. If they are improperly specified an error will
/// result and no action will be pefrormed. The manifold glue operation often found in implementations of the euler
/// operators can be emulated by performing an additional KillFace after the GlueFace to remove the surviving face f1.
/// </summary>
/// <param name="fu1"></param>
/// <param name="eu1"></param>
/// <param name="fu2"></param>
/// <param name="eu2"></param>
void GlueFace(Face* f1, Edge* e1, Face* f2, Edge* e2);

/// <summary>
/// Splits the specified edge e into two conencted edges - e and newe. A new vert newv is created between these two
/// edges. The optional parameter v, if specified, designates which vertex of the edge e will be found on the new edge.
/// For manifold situations, the effoect of this operator could be simulated by application of the KillEdge operator
/// followed by ManifoldMakeEdgeVertex and ManifoldMakeEdge operators, but unlike EdgeSplit, edge e would be entirely
/// replaced rather than modified in place, and by side effect, a face could be deleted and replaced with a new one,
/// perhaps shifting ownership of interior loops. IN non-manifold situations where edge is used three or more times by
/// one or more faces, KillEdge will not allow deletion of the edge since non-manifold faces sould be created, so
/// EdgeSplit is the only alternative.
/// </summary>
/// <param name="e"></param>
/// <param name="v"></param>
/// <param name="newe"></param>
/// <param name="newv"></param>
void EdgeSplit(Edge* e, Vert* v, Edge* newe, Vert* newv);

/// <summary>
/// Squeezes the ends of the specified edge e together, deleting the edge and a vertex while preserving adjecencies.
/// the optional parameter v, if specified, designates which vertex of the edge e will survive. in any case, the
/// surviving vertex is indicated by the vsurvivor return parameter. For manifold situations the effect of this operator
/// could be simulated ba application to the KillEdge operator, followed by the GlueVert Operator, but unlike
/// EdgeSqueeze ,by side effect a face could be deleted and replaced with a new one, perhaps shifting ownership of
/// interior loops.
/// </summary>
/// <param name="e"></param>
/// <param name="v"></param>
/// <param name="vSurvivor"></param>
void EdgeSqueeze(Edge* e, Vert* v, Vert* vSurvivor);

/// <summary>
/// Creates a new wire edge newe which connects vert v with a new vert newv.
/// </summary>
/// <param name="v"></param>
/// <param name="newe"></param>
/// <param name="newv"></param>
void MakeEdgeVert(Vert* v, Edge* newe, Vert* newv);

/// <summary>
/// Creates a new wire edge, newe, between the specified vertices v1 and v2.
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <param name="newe"></param>
void MakeEdge(Vert* v1, Vert* v2, Edge* newe);

/// <summary>
/// Creates a new loop where each orientation is specified in an ordered list of edges and verts.
/// </summary>
/// <param name="edges">Ordered list of edges which form a closed loop</param>
/// <param name="verts">Ordered list of verts which form a closed loop</param>
/// <param name="newl"></param>
void MakeLoop(std::vector<Edge*> edges, std::vector<Vert*> verts, Loop* newl);

/// <summary>
/// Creates a new face using the specified loops, where each element in loops is a pointer to the first item in the
/// closed circuit of loops. This methods allows for creation of faces with holes/multiple loops. Use the MakeLoop
/// EulerOp to create all necessary loops. Loops must be traversable using fNext/fPrev. 
/// Loops must be already added to their edges lists.
/// </summary>
/// <param name="loops"></param>
/// <param name="newf"></param>
void MakeFace(std::vector<Loop*> loops, Face* newf);

/// <summary>
/// Creates a new face newf with its single loop newl bounded by the single circuit of edges as specified in edges. The
/// list of edges specified by edges must form a true circuit or an error will result and no action will be performed.
/// </summary>
/// <param name="edges"></param>
/// <param name="newf"></param>
void MakeFace(std::vector<Edge*> edges, Face* newf);

/// <summary>
/// Deletes the face f and all loops associated with it. It does not delete any edges or vertices.
/// This is NOT equivalent to calling ~Face()!
/// </summary>
/// <param name="f"></param>
void KillFace(Face* f);

/// <summary>
/// Creates a manifold edge and a vertex. The new edge newe starts at existing vertex v and ends at the new vertex newv.
/// The edge and vertex are created in the face F.
/// </summary>
/// <param name="v"></param>
/// <param name="f"></param>
/// <param name="newe"></param>
/// <param name="newv"></param>
void ManifoldMakeEdgeVert(Vert* v, Face* f, Edge* newe, Vert* newv);

/// <summary>
/// Creates an edge between the existing vertices v1 and v2. The edge is created in the face f. Verts v1 and v2 must be
///  on the boundary of face f or an error will result and no action will be performed.
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <param name="f"></param>
/// <param name="newe"></param>
/// <param name="newf"></param>
/// <param name="newl"></param>
void ManifoldMakeEdge(Vert* v1, Vert* v2, Face* f, Edge* newe, Face* newf, Loop* newl);

/// <summary>
/// Deletes all elements present in its mesh, returning it to an empty state.
/// This is NOT equivalent to calling ~Mesh()!
/// </summary>
/// <param name="m"></param>
void KillMesh(Mesh* m);

} // namespace Core
} // namespace Aoba

#endif