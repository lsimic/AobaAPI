#ifndef AOBA_OPS_CREATE_HPP
#define AOBA_OPS_CREATE_HPP

#include "AobaAPI/Core.hpp"
#include "AobaAPI/Math.hpp"

#include <vector>

namespace Aoba {
namespace Ops {

class CreateCircleResult {
  public:
    std::vector<Core::Vert*> verts;
    std::vector<Core::Edge*> edges;
};

const CreateCircleResult CreateCircle(Core::Mesh* m, unsigned vertCount, float radius);

class CreateCubeResult {
  public:
    std::vector<Core::Vert*> verts;
    std::vector<Core::Edge*> edges;
    std::vector<Core::Face*> Faces;
};

const CreateCubeResult CreateCube(Core::Mesh* m, float size);

class CreateGridResult {
  public:
    std::vector<Core::Vert*> innerVerts;
    std::vector<Core::Vert*> outerVerts;
    std::vector<Core::Edge*> innerEdges;
    std::vector<Core::Edge*> outerEdges;
    std::vector<Core::Face*> innerFaces;
    std::vector<Core::Face*> outerFaces;
};

const CreateGridResult CreateGrid(Core::Mesh* m, int divisionsX, int divisionsY, float sizeX, float sizeY);

class CreateIcoSphereResult {
  public:
    std::vector<Core::Vert*> verts;
    std::vector<Core::Edge*> edges;
    std::vector<Core::Face*> Faces;
};

const CreateIcoSphereResult CreateIcoSphere(Core::Mesh* m, float radius, unsigned divisions);

class CreateUvSphereResult {
  public:
    std::vector<Core::Vert*> verts;
    std::vector<Core::Edge*> edges;
    std::vector<Core::Face*> Faces;
};

const CreateUvSphereResult CreateUVSphere(Core::Mesh* m, float radius, std::size_t rings, std::size_t segments);

Core::Vert* CreateVert(Core::Mesh* m, const Math::Vec3 co);

} // namespace Ops
} // namespace Aoba

#endif