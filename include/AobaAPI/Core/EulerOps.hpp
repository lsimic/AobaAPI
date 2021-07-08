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
/// Deletes the Edge e and joins the faces which touch upon Edge e into a single face. If the operation would result
/// in the creation of a non manifold face (which occours when edge is used three or more times), or if the faces have
/// other common edges an error will result and no operation will be performed. The parameter fSurvivor specifies which
/// face will be the surviving face. Loops which use the edge e are also deleted. 
/// </summary>
/// <param name="e">Edge which will be dissolved</param>
/// <param name="fSurvivor">Surviving face of the operation</param>
/// <exception cref="std::invalid_argument">
/// Thrown if edge e does not have exactly two adjacent faces.
/// Thrown if edge e is not the only edge shared by it's faces.
/// </exception>
void DissolveEdge(Mesh* m, Edge* e, Face* fSurvivor);

// TODO: should also make a DissolveVert operator, but i'm not sure about the behaviour
// what happens if Vert is used by three or more wire edges?
// what happens if vert is boundary?

/// <summary>
/// Splits the specified edge e into two conencted edges - e and newe. A new vert newv is created between these two
/// edges. The parameter v designates which vertex of the edge e will be found on the new edge.
/// </summary>
/// <param name="e">Edge which will be split</param>
/// <param name="v">Vert of the edge e which will be found on the new edge</param>
/// <param name="newe">New edge which will be placed between v and newv</param>
/// <param name="newv">New vert which will be placed between e and newe</param>
void EdgeSplit(Mesh* m, Edge* e, Vert* v, Edge* newe, Vert* newv);

/// <summary>
/// Squeezes the ends of the specified edge e together, deleting the edge and a vertex while preserving adjecencies.
/// The parameter vSurvivor designates which vertex of the edge e will survive. If the squeeze operation will result
/// in creation of a dissalowed condition(multigraph, self loop, face with less than 3 edges), faces, loops, verts and
/// edges are deleted as necessary. 
/// </summary>
/// <remarks>
/// This operator is implemented using the <see cref="GlueVert"/> operator. 
/// </remarks>
/// <param name="e">Edge which will be squeezed</param>
/// <param name="vSurvivor">The surviving vertex</param>
void EdgeSqueeze(Mesh* m, Edge* e, Vert* vSurvivor);

/// <summary>
/// Merges the Edge e1 together with the Edge e2 preserving the adhecencies of elements. The Edge e1 is the
/// surviving Edge. The Edge e2 and any of its vertices which are not shared with the Edge e1 are deleted. If the two 
/// edges do not share any vertices, vert v1 of e1 is glued to the vert v1 of e2. The same happens for vert v2. If the
/// edges have some geometry between them(other edges, faces, loops...) it will be deleted as necessary to avoid
/// creation of disallowed conditions
/// </summary>
/// <remarks>
/// This operator is implemented using the <see cref="GlueVert"/> operator.
/// </remarks>
/// <param name="e1">First edge to glue, surviving edge</param>
/// <param name="e2">Other edge to glue</param>
void GlueEdge(Mesh* m, Edge* e1, Edge* e2);

/// <summary>
/// Merges the single loop faces f1 and f2 together, preserving the adjecencies of elements. e1 and e22 specify exactly
/// how and in what direction the two loop boundaries match up. The loops are glued togerher with the edges in
/// opposite orientation. The face f1 is the surviving face. The face f2 and its loop, edges and vertices not shared
/// with the face f1 are deleted. The faces specified by both f1 and f2 must each have one only loop with the same
/// number of edges or an error will result and no action will be performed. Note that if the faces f1 and f2 share 
/// any edges or two or more vertices the acceptable orientations for glue operations may already be fixed. 
/// If they are improperly specified an error will result and no action will be pefrormed.
/// </summary>
/// <remarks>
/// This operator is implemented using the <see cref="GlueVert"/> operator.
/// </remarks>
/// <param name="f1">Surviving face</param>
/// <param name="e1">Edge of the surviving face to specify orientation.</param>
/// <param name="f2">Other face</param>
/// <param name="e2">Edge of the other face to specify orientation</param>
/// <exception cref="std::invalid_argument">
/// Thrown if edge e1 and/or e2 do not belong to faces f1 and/or f2.
/// Thrown if the face loops do not match the specified orientation.
/// </exception>
void GlueFace(Mesh* m, Face* f1, Edge* e1, Face* f2, Edge* e2);

/// <summary>
/// Merges the vertices of v1 and v2 together, preserving the adjecencies of elements, the Vert of v1 is the
/// surviving Vert. The Vert of v2 is deleted. If the operation will result in the creation of disallowed 
/// topology configurations, elements(faces, edges, loops) are deleted as necessary. 
/// </summary>
/// <param name="v1">The surviving vert</param>
/// <param name="v2">The other vert</param>
void GlueVert(Mesh* m, Vert* v1, Vert* v2);

/// <summary>
/// Deletes the Edge e and Kills any faces which are adjecent to it.
/// This is NOT equivalent to calling ~Edge()!
/// </summary>
/// <param name="e">Edge to be killed</param>
void KillEdge(Mesh* m, Edge* e);

/// <summary>
/// Deletes the face f and all loops associated with it. It does not delete any edges or vertices.
/// This is NOT equivalent to calling ~Face()!
/// </summary>
/// <param name="f">Face to be killed</param>
void KillFace(Mesh* m, Face* f);

/// <summary>
/// Deletes all elements present in its mesh, returning it to an empty state.
/// This is NOT equivalent to calling ~Mesh()!
/// </summary>
/// <param name="m">Mesh to be killed</param>
void KillMesh(Mesh* m);

/// <summary>
/// Deletes the Vert v and Kills any Edges which are adjecent, also killing Faces as necessary. 
/// This is NOT equivalent to calling ~Vert()!
/// </summary>
/// <param name="v">Vert to bi killed</param>
void KillVert(Mesh* m, Vert* v);

/// <summary>
/// Creates a new wire edge, newe, between the specified vertices v1 and v2.
/// </summary>
/// <param name="v1">First vert of the edge</param>
/// <param name="v2">Second vert of the edge</param>
/// <param name="newe">The new edge</param>
/// <exception cref="std::invalid_argument">
/// Thrown if vert v1 is equal to vert v2.
/// Thrown if an edge already exists between the given verts.
/// </exception>
void MakeEdge(Mesh* m, Vert* v1, Vert* v2, Edge* newe);

/// <summary>
/// Creates a new wire edge newe which connects vert v with a new vert newv.
/// </summary>
/// <param name="v">Existing vert</param>
/// <param name="newe">The new vert</param>
/// <param name="newv">The new edge</param>
void MakeEdgeVert(Mesh* m, Vert* v, Edge* newe, Vert* newv);

/// <summary>
/// Checks wether the provided edges and verts form a valid loop. 
/// loop is valid if: Each edge is used only once, each vert is used only once and all provided edges and verts form a 
/// closed circuit. 
/// There is no need to always validate the loop before creating. You can skip if you *know* that the loop is valid.
/// The only default high-level op where this is used is MakeFace. 
/// </summary>
/// <param name="edges">ordered edges which form the loop.</param>
/// <param name="verts">Ordered verts which form the loop.</param>
/// <returns>True if loop is valid, otherwise false.</returns>
bool ValidateLoop(Mesh* m, std::vector<Edge*> edges, std::vector<Vert*> verts);

/// <summary>
/// Creates a new face using the specified loops, where loop is a pointer to the first element in the loop list.
/// Use the <see cref="MakeLoop"/> EulerOp to create all necessary loops. Loops must be traversable using fNext/fPrev.
/// Loops must be already added to their edges lists (MakeLoop euler operator does this).
/// </summary>
/// <param name="loops">Loops which form the face boundary</param>
/// <param name="newf">The new face</param>
void MakeFace(Mesh* m, Loop* loop, Face* newf);

/// <summary>
/// Creates a new loop where each orientation is specified in an ordered list of edges and verts.
/// </summary>
/// <param name="edges">Ordered list of edges which form a closed loop</param>
/// <param name="verts">Ordered list of verts which form a closed loop</param>
/// <param name="newl">The new loop</param>
/// /// <exception cref="std::invalid_argument">
/// Thrown if there are less than three edges or verts.
/// </exception>
void MakeLoop(Mesh* m, std::vector<Edge*> edges, std::vector<Vert*> verts, Loop* newl);

/// <summary>
/// Creates a new loop where each orientation is specified in an ordered list of edges and verts.
/// </summary>
/// <param name="edges">Ordered list of edges which form a closed loop</param>
/// <param name="verts">Ordered list of verts which form a closed loop</param>
/// <param name="newLoops">List of new loops</param>
/// /// <exception cref="std::invalid_argument">
/// Thrown if there are less than three edges or verts, or the input sizes do not match
/// </exception>
void MakeLoop(Mesh* m, std::vector<Edge*> edges, std::vector<Vert*> verts, std::vector<Loop*> newLoops);

/// <summary>
/// Creates a new Vert newv in an existing mesh m.
/// </summary>
/// <param name="m">Mesh inside which the vert will be created</param>
/// <param name="newv">The new vert</param>
void MakeVert(Mesh* m, Vert* newv);

/// <summary>
/// Creates an edge between the existing vertices v1 and v2. The edge is created in the face f. Verts v1 and v2 must be
///  on the boundary of the face f or an error will result and no action will be performed.
/// </summary>
/// <param name="v1">First vert of the edge</param>
/// <param name="v2">Other vert of the edge</param>
/// <param name="f">Existing face where v1 and v2 are elements of the boundary</param>
/// <param name="newe">The new edge</param>
/// <param name="newf">The new face</param>
/// <exception cref="std::invalid_argument">
/// Thrown if edge v1, v2 are not found on the face boundary
/// Thrown if an edge already exists between v1 and v2. 
/// </exception>
void ManifoldMakeEdge(Mesh* m, Vert* v1, Vert* v2, Face* f, Edge* newe, Face* newf);

/// <summary>
/// Joins the mesh m2 into the mesh m1, deleting the mesh m2. Referecens to all elements of mesh 2 remain valid. 
/// </summary>
/// <param name="m1">The surviving mesh</param>
/// <param name="m2">The other mesh</param>
void JoinMesh(Mesh* m1, Mesh* m2);

} // namespace Core
} // namespace Aoba

#endif