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
/// in the creation of a non manifold face (which occours when edge is used three or more times) an error will result
/// and no operation will be performed. The optional parameter f specifies which face will be the surviving face. In
/// any case, the fSurvivor return parameter will be populated.
/// </summary>
/// <param name="e"></param>
/// <param name="f"></param>
/// <param name="fSurvivor"></param>
void DissolveEdge(Edge* e, Face* f, Face* fSurvivor);

// TODO: should also make a DissolveVert operator, but i'm not sure about the behaviour
// what happens if Vert is used by three or more wire edges?
// what happens if vert is boundary?

/// <summary>
/// Splits the specified edge e into two conencted edges - e and newe. A new vert newv is created between these two
/// edges. The optional parameter v, if specified, designates which vertex of the edge e will be found on the new edge.
/// </summary>
/// <param name="e"></param>
/// <param name="v"></param>
/// <param name="newe"></param>
/// <param name="newv"></param>
void EdgeSplit(Edge* e, Vert* v, Edge* newe, Vert* newv);

/// <summary>
/// Squeezes the ends of the specified edge e together, deleting the edge and a vertex while preserving adjecencies.
/// the parameter vSurvivor designates which vertex of the edge e will survive.
/// </summary>
/// <param name="e"></param>
/// <param name="vSurvivor"></param>
void EdgeSqueeze(Edge* e, Vert* vSurvivor);

/// <summary>
/// Merges the Edge e1 together with the Edge e2 preserving the adhecencies of elements. The Edge e1 is the
/// surviving Edge. The Edge e2 and any of its vertices which are not shared with the Edge e1 are deleted.
/// </summary>
/// <param name="e1"></param>
/// <param name="e2"></param>
void GlueEdge(Edge* e1, Edge* e2);

/// <summary>
/// Merges the single loop faces f1 and f2 together, preserving the adjecencies of elements. e1 and e22 specify exactly
/// how and in what direction the two loop boundaries match up. The loops are glued togerher with the edges in
/// opposite orientation. The face f1 is the surviving face. The face f2 and its loop, edges and vertices not shared
/// with the face f1 are deleted. The faces specified by both f1 and f2 must each have one only loop with the same
/// number of edges with self loop, isthmus and strut edges in an identical order in both loops or an error will result
///  and no action will be performed. Note that if the faces f1 and f2 share any edges or two or more vertices the
/// acceptable orientations for glue operations may already be fixed. If they are improperly specified an error will
/// result and no action will be pefrormed.
/// </summary>
/// <param name="fu1"></param>
/// <param name="eu1"></param>
/// <param name="fu2"></param>
/// <param name="eu2"></param>
void GlueFace(Face* f1, Edge* e1, Face* f2, Edge* e2);

/// <summary>
/// Merges the vertices of v1 and v2 together, preserving the adjecencies of elements, the Vert of v1 is the
/// surviving Vert. The Vert of v2 is deleted.
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
void GlueVert(Vert* v1, Vert* v2);

/// <summary>
/// Deletes the Edge e and any faces which touch upon it, deleting loops as necessary.
/// This is NOT equivalent to calling ~Edge()!
/// </summary>
/// <param name="e"></param>
void KillEdge(Edge* e);

/// <summary>
/// Deletes the face f and all loops associated with it. It does not delete any edges or vertices.
/// This is NOT equivalent to calling ~Face()!
/// </summary>
/// <param name="f"></param>
void KillFace(Face* f);

/// <summary>
/// Deletes all elements present in its mesh, returning it to an empty state.
/// This is NOT equivalent to calling ~Mesh()!
/// </summary>
/// <param name="m"></param>
void KillMesh(Mesh* m);

/// <summary>
/// Deletes the Vert v and any Edges which touch upon it, deleting Loops and Faces as necessary.
/// This is NOT equivalent to calling ~Vert()!
/// </summary>
/// <param name="v"></param>
void KillVert(Vert* v);

/// <summary>
/// Creates a new wire edge, newe, between the specified vertices v1 and v2.
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <param name="newe"></param>
void MakeEdge(Vert* v1, Vert* v2, Edge* newe);

/// <summary>
/// Creates a new wire edge newe which connects vert v with a new vert newv.
/// </summary>
/// <param name="v"></param>
/// <param name="newe"></param>
/// <param name="newv"></param>
void MakeEdgeVert(Vert* v, Edge* newe, Vert* newv);

/// <summary>
/// Checks wether the provided edges and verts form a valid loop. 
/// loop is valid if: Each edge is used only once, each vert is used only once and all provided edges and verts form a 
/// closed circuit. 
/// There is no need to always validate the loop before creating. You can skip if you *know* that the loop is valid.
/// The only default high-level op where this is used is MakeFace. 
/// </summary>
/// <param name="edges"></param>
/// <param name="verts"></param>
/// <returns></returns>
bool ValidateLoop(std::vector<Edge*> edges, std::vector<Vert*> verts);

/// <summary>
/// Creates a new face using the specified loops, where each element in loops parameter is a pointer to the first
/// item in the closed circuit of loops. This methods allows for creation of faces with holes/multiple loops.
/// Use the MakeLoop EulerOp to create all necessary loops. Loops must be traversable using fNext/fPrev.
/// Loops must be already added to their edges lists (MakeLoop euler operator does this).
/// </summary>
/// <param name="loops"></param>
/// <param name="newf"></param>
void MakeFace(Loop* loop, Face* newf);

/// <summary>
/// Creates a new loop where each orientation is specified in an ordered list of edges and verts.
/// </summary>
/// <param name="edges">Ordered list of edges which form a closed loop</param>
/// <param name="verts">Ordered list of verts which form a closed loop</param>
/// <param name="newl"></param>
void MakeLoop(std::vector<Edge*> edges, std::vector<Vert*> verts, Loop* newl);

/// <summary>
/// Creates a new Vert newv in an existing mesh m.
/// </summary>
/// <param name="m"></param>
/// <param name="newv"></param>
void MakeVert(Mesh* m, Vert* newv);

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
void ManifoldMakeEdge(Vert* v1, Vert* v2, Face* f, Edge* newe, Face* newf);

} // namespace Core
} // namespace Aoba

#endif