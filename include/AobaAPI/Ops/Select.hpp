#ifndef AOBA_OPS_SELECT_HPP
#define AOBA_OPS_SELECT_HPP

#include "AobaAPI/Core.hpp"
#include "AobaAPI/Math.hpp"

#include <vector>

namespace Aoba {
namespace Ops {

class SelectResult {
  public:
    std::vector<Core::Vert*> verts; // selected verts
    std::vector<Core::Edge*> edges; // selected edges
    std::vector<Core::Face*> faces; // selected faces
};

/// <summary>
/// Grow the input geometry using the defined step. The output geometry is returned together with input elements
/// Faces and edges are also selected if their boundaries are selected(both input and grown)
/// </summary>
/// <param name="m">Mesh on which to operate on</param>
/// <param name="verts">Input verts</param>
/// <param name="edges">Input edges</param>
/// <param name="faces">Input faces</param>
/// <param name="faceStep">Wether to step using faces(or edges)</param>
/// <returns>Newly selected geometry along with previously selected geometry</returns>
const SelectResult SelectExtend(Core::Mesh* m, const std::vector<Core::Vert*>& verts,
    const std::vector<Core::Edge*>& edges, const std::vector<Core::Face*>& faces, bool faceStep);

/// <summary>
/// Reduce the input geometry using the defined step.
/// Faces and edges are also selected if their boundaries are selected(both input and grown)
/// </summary>
/// <param name="m">Mesh on which to operate on</param>
/// <param name="verts">Input verts</param>
/// <param name="edges">Input edges</param>
/// <param name="faces">Input faces</param>
/// <param name="faceStep">Wether to step using faces(or edges)</param>
/// <returns>Selected geometry. can be empty. </returns>
const SelectResult SelectReduce(Core::Mesh* m, const std::vector<Core::Vert*>& verts,
    const std::vector<Core::Edge*>& edges, const std::vector<Core::Face*>& faces, bool faceStep);

/// <summary>
/// Transform input edges into other elements - verts and faces
/// </summary>
/// <param name="m">Mesh on which to operate on</param>
/// <param name="edges">Input edges</param>
/// <returns>The transformed selection. edges countain input edges.</returns>
const SelectResult SelectFromEdges(Core::Mesh* m, const std::vector<Core::Edge*>& edges);

/// <summary>
/// Transform input faces into other elements - edged and verts
/// </summary>
/// <param name="m">Mesh on which to operate on</param>
/// <param name="faces">Input faces</param>
/// <returns>The transformed selection. faces countain input faces.</returns>
const SelectResult SelectFromFaces(Core::Mesh* m, const std::vector<Core::Face*>& faces);

/// <summary>
/// Transform input verts into other elements - edges and faces
/// </summary>
/// <param name="m">Mesh on which to operate on</param>
/// <param name="verts">Input verts</param>
/// <returns>The transformed selection. verts countain input verts.</returns>
const SelectResult SelectFromVerts(Core::Mesh* m, const std::vector<Core::Vert*>& verts);

} // namespace Ops
} // namespace Aoba

#endif