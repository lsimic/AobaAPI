#include "AobaAPI/Ops/Create.hpp"

#include <cmath>

namespace Aoba {
namespace Ops {

const CreateCubeResult CreateCube(Core::Mesh* m, float size) {
    // first create all vertices
    std::vector<Core::Vert*> verts = std::vector<Core::Vert*>();
    verts.reserve(8);

    // add new verts, add them to mesh
    for(int i = 0; i < 8; ++i) {
        Core::Vert* newv = new Core::Vert();
        Core::MakeVert(m, newv);
        verts.push_back(newv);
    }

    // set up vert coordinates
    // perhaps this could be done inside a loop, but this is readable and efficient
    float halfSize = size / 2;
    verts.at(0)->co = Math::Vec3(-halfSize, -halfSize, -halfSize);
    verts.at(1)->co = Math::Vec3(halfSize, -halfSize, -halfSize);
    verts.at(2)->co = Math::Vec3(halfSize, -halfSize, halfSize);
    verts.at(3)->co = Math::Vec3(-halfSize, -halfSize, halfSize);
    verts.at(4)->co = Math::Vec3(-halfSize, halfSize, -halfSize);
    verts.at(5)->co = Math::Vec3(halfSize, halfSize, -halfSize);
    verts.at(6)->co = Math::Vec3(halfSize, halfSize, halfSize);
    verts.at(7)->co = Math::Vec3(-halfSize, halfSize, halfSize);

    // create all edges
    std::vector<Core::Edge*> edges = std::vector<Core::Edge*>();
    edges.reserve(12);
    for(std::size_t i = 0; i < 4; ++i) {
        Core::Edge* newe = new Core::Edge();
        if(i == 3) {
            Core::MakeEdge(verts.at(i), verts.at(0), newe);
        } else {
            Core::MakeEdge(verts.at(i), verts.at(i + 1), newe);
        }
        edges.push_back(newe);
    }
    for(std::size_t i = 4; i < 8; ++i) {
        Core::Edge* newe = new Core::Edge();
        if(i == 7) {
            Core::MakeEdge(verts.at(i), verts.at(4), newe);
        } else {
            Core::MakeEdge(verts.at(i), verts.at(i + 1), newe);
        }
        edges.push_back(newe);
    }
    for(std::size_t i = 0; i < 4; ++i) {
        Core::Edge* newe = new Core::Edge();
        Core::MakeEdge(verts.at(i), verts.at(i + 4), newe);
        edges.push_back(newe);
    }

    // create faces
    std::vector<Core::Face*> faces = std::vector<Core::Face*>();
    faces.reserve(6);
    for(int i = 0; i < 6; ++i) {
        faces.push_back(new Core::Face());
    }

    // perhaps this could be done in a loop, but this works for now.
    // not the most elegant solution
    Core::Loop* newl = new Core::Loop();
    std::vector<Core::Edge*> loopEdges = {edges.at(0), edges.at(1), edges.at(2), edges.at(3)};
    std::vector<Core::Vert*> loopVerts = {verts.at(0), verts.at(1), verts.at(2), verts.at(3)};
    Core::MakeLoop(loopEdges, loopVerts, newl);
    Core::MakeFace(newl, faces.at(0));

    newl = new Core::Loop();
    loopEdges = {edges.at(4), edges.at(7), edges.at(6), edges.at(5)};
    loopVerts = {verts.at(5), verts.at(4), verts.at(7), verts.at(6)};
    Core::MakeLoop(loopEdges, loopVerts, newl);
    Core::MakeFace(newl, faces.at(1));

    newl = new Core::Loop();
    loopEdges = {edges.at(9), edges.at(5), edges.at(10), edges.at(1)};
    loopVerts = {verts.at(1), verts.at(5), verts.at(6), verts.at(2)};
    Core::MakeLoop(loopEdges, loopVerts, newl);
    Core::MakeFace(newl, faces.at(2));

    newl = new Core::Loop();
    loopEdges = {edges.at(8), edges.at(3), edges.at(11), edges.at(7)};
    loopVerts = {verts.at(4), verts.at(0), verts.at(3), verts.at(7)};
    Core::MakeLoop(loopEdges, loopVerts, newl);
    Core::MakeFace(newl, faces.at(3));

    newl = new Core::Loop();
    loopEdges = {edges.at(10), edges.at(6), edges.at(11), edges.at(2)};
    loopVerts = {verts.at(2), verts.at(6), verts.at(7), verts.at(3)};
    Core::MakeLoop(loopEdges, loopVerts, newl);
    Core::MakeFace(newl, faces.at(4));

    newl = new Core::Loop();
    loopEdges = {edges.at(8), edges.at(4), edges.at(9), edges.at(0)};
    loopVerts = {verts.at(0), verts.at(4), verts.at(5), verts.at(1)};
    Core::MakeLoop(loopEdges, loopVerts, newl);
    Core::MakeFace(newl, faces.at(5));

    CreateCubeResult result = CreateCubeResult();
    result.edges = edges;
    result.Faces = faces;
    result.verts = verts;

    return result;
}

} // namespace Ops
} // namespace Aoba
