#include "AobaAPI/IO/IndexMesh.hpp"

namespace Aoba {
namespace IO {

void IndexMesh::FromMesh(Core::Mesh* m) {
    std::vector<Core::Vert*> mVerts = m->Verts();
    std::vector<Core::Edge*> mEdges = m->Edges();
    std::vector<Core::Face*> mFaces = m->Faces();

    vertexCoords = std::vector<float>();
    edges = std::vector<std::size_t>();
    triangles = std::vector<std::size_t>();

    vertexCoords.reserve(mVerts.size() * 3);
    edges.reserve(mEdges.size() * 2);
    triangles.reserve(mFaces.size() * 2); // expecting mostly quads

    // populate all vertex coordinates, set vertex index.
    std::size_t vertIdx = 0;
    for(Core::Vert* v : mVerts) {
        vertexCoords.push_back(v->co.x);
        vertexCoords.push_back(v->co.y);
        vertexCoords.push_back(v->co.z);
        v->index = vertIdx;
        vertIdx++;
    }

    // populate edges. this will include both wire and non-wire edges together
    for(Core::Edge* e : mEdges) {
        edges.push_back(e->V1()->index);
        edges.push_back(e->V2()->index);
    }

    // populate faces
    // use simple triangle-fan triangulation for non-triangular faces.
    for(Core::Face* f : mFaces) {
        std::vector<Core::Loop*> fLoops = f->Loops();
        std::size_t idx = 1;
        while(idx + 1 < fLoops.size()) {
            triangles.push_back(fLoops.at(0)->LoopVert()->index);
            triangles.push_back(fLoops.at(idx)->LoopVert()->index);
            triangles.push_back(fLoops.at(idx + 1)->LoopVert()->index);
            ++idx;
        }
    }

    // reset vertex index.
    for(Core::Vert* v : mVerts) {
        v->index = 0;
    }
}

} // namespace IO
} // namespace Aoba