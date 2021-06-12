#ifndef AOBA_IO_INDEX_MESH_HPP
#define AOBA_IO_INDEX_MESH_HPP

#include "../Core.hpp"

namespace Aoba {
namespace IO {

class IndexMesh {
  public:
    std::vector<float> vertexCoords;    // packed vertex coordinates, in x,y,z order
    std::vector<std::size_t> edges;     // packed edge vertex indices, in v1,v2 order
    std::vector<std::size_t> triangles; // indices of triangle coordinates, v1,v2,v3 order

    /// <summary>
    /// populate the data of this index mesh from mesh.
    /// </summary>
    /// <param name="m">Mesh to convert to index-based representation</param>
    void FromMesh(Core::Mesh* m);
};

} // namespace IO
} // namespace Aoba

#endif