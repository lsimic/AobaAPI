#ifndef AOBA_OPS_CREATE_HPP
#define AOBA_OPS_CREATE_HPP

#include "AobaAPI/Core.hpp"
#include "AobaAPI/Math.hpp"

#include <vector>

namespace Aoba {
namespace Ops {

class CreateCircleResult {
  public:
    std::vector<Core::Vert*> verts; // circle verts
    std::vector<Core::Edge*> edges; // circle edges
};

/// <summary>
/// Create circle of specified radius at the world origin
/// </summary>
/// <param name="m">Mesh on which to operate on</param>
/// <param name="vertCount">Number of verts which form the circle</param>
/// <param name="radius">Radius of the circle</param>
/// <returns>New verts and edges</returns>
const CreateCircleResult CreateCircle(Core::Mesh* m, unsigned vertCount, float radius);

class CreateCubeResult {
  public:
    std::vector<Core::Vert*> verts; // cube verts
    std::vector<Core::Edge*> edges; // cube edges
    std::vector<Core::Face*> Faces; // cube faces
};

/// <summary>
/// Create cube of specified size centered at the world origin
/// </summary>
/// <param name="m">Mesh on which to operate on</param>
/// <param name="size">Size of the cube(same for all dimensions)</param>
/// <returns>new verts, edges and faces</returns>
const CreateCubeResult CreateCube(Core::Mesh* m, float size);

class CreateGridResult {
  public:
    std::vector<Core::Vert*> innerVerts; // grid verts which do not touch the boundary
    std::vector<Core::Vert*> outerVerts; // grid verts found on the boundary
    std::vector<Core::Edge*> innerEdges; // grid verts not found on the boundary
    std::vector<Core::Edge*> outerEdges; // grid verts found on the boundary, forming the outer loop.
    std::vector<Core::Face*> innerFaces; // grid faces not found on the boundary
    std::vector<Core::Face*> outerFaces; // grid faces found on the boundary
};

/// <summary>
/// Create a new grid at the world origin, with specified divisions and size for each axis, in the x-y plane
/// if divisions are 0, creates a plane consisting of 4 verts, 4 edges and a single face
/// </summary>
/// <param name="m">Mesh on which to operate on</param>
/// <param name="divisionsX">Number of divisions on the x axis</param>
/// <param name="divisionsY">Number of divisions on the y axis</param>
/// <param name="sizeX">Size on the x axis</param>
/// <param name="sizeY">Size on the y axis</param>
/// <returns>new verts, edges and faces</returns>
const CreateGridResult CreateGrid(Core::Mesh* m, int divisionsX, int divisionsY, float sizeX, float sizeY);

class CreateIcoSphereResult {
  public:
    std::vector<Core::Vert*> verts; // sphere verts
    std::vector<Core::Edge*> edges; // sphere edges
    std::vector<Core::Face*> Faces; // sphere faces
};

/// <summary>
/// TODO:
/// Create a new IcoSphere of the given radius, centered at the world origin
/// </summary>
/// <param name="m">Mesh on which to operate on</param>
/// <param name="radius">Radius of the sphere</param>
/// <param name="divisions">Number of divisions(for each edge)</param>
/// <returns>new edges, verts and faces</returns>
/// TODO:
const CreateIcoSphereResult CreateIcoSphere(Core::Mesh* m, float radius, unsigned divisions);

class CreateUvSphereResult {
  public:
    std::vector<Core::Vert*> verts; // sphere verts
    std::vector<Core::Edge*> edges; // sphere edges
    std::vector<Core::Face*> Faces; // sphere faces
};

/// <summary>
/// Creates a new UV sphere centered at the world origin
/// </summary>
/// <param name="m">Mesh on which to operate on</param>
/// <param name="radius">Radius of the sphere</param>
/// <param name="rings">Number of rings(vertical slices)</param>
/// <param name="segments">Number of slices(orthogonal to x-y plane)</param>
/// <returns>New edges, verts and faces</returns>
const CreateUvSphereResult CreateUVSphere(Core::Mesh* m, float radius, std::size_t rings, std::size_t segments);

/// <summary>
/// Create a single vertex at the given coordinates.
/// </summary>
/// <param name="m">Mesh on which to operate on</param>
/// <param name="co">Coordinate of the new vertex</param>
/// <returns>The new vertex</returns>
Core::Vert* CreateVert(Core::Mesh* m, const Math::Vec3 co);

} // namespace Ops
} // namespace Aoba

#endif