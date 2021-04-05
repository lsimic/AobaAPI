#include "AobaAPI/Ops/Create.hpp"

namespace Aoba {
namespace Ops {

const CreateGridResult CreateGrid(Core::Mesh* m, int divisionsX, int divisionsY, float sizeX, float sizeY) {
    std::size_t divsX = divisionsX;
    std::size_t divsY = divisionsY;
    // Create verts.
    std::vector<Core::Vert*> verts = std::vector<Core::Vert*>();
    verts.reserve((2 + divsX) * (2 + divsY));
    std::vector<Core::Vert*> boundaryVerts = std::vector<Core::Vert*>();
    boundaryVerts.reserve(divsX + 2 * (2 + divsX));
    std::vector<Core::Vert*> innerVerts = std::vector<Core::Vert*>();
    innerVerts.reserve(divsX * divsY);

    float xStep = sizeX / (divsX + 1);
    float yStep = sizeY / (divsY + 1);
    for(int x = 0; x <= divsX + 1; ++x) {
        for(int y = 0; y <= divsY + 1; ++y) {
            Core::Vert* newv = new Core::Vert();
            Core::MakeVert(m, newv);
            newv->co = Math::Vec3(-sizeX / 2 + x * xStep, -sizeY / 2 + y * yStep, 0);
            verts.push_back(newv);

            // push to inner/boundary vert
            if(x == 0 || x == divsX + 1 || y == 0 || y == divsY + 1) {
                boundaryVerts.push_back(newv);
            } else {
                innerVerts.push_back(newv);
            }
        }
    }

    std::vector<Core::Edge*> boundaryEdges = std::vector<Core::Edge*>();
    boundaryEdges.reserve(2 * (divsX + 2 + divsY));
    std::vector<Core::Edge*> innerEdges = std::vector<Core::Edge*>();
    innerEdges.reserve(divsX * (1 + divsY) + divsY * (1 + divsX));

    std::vector<Core::Face*> boundaryFaces = std::vector<Core::Face*>();
    std::size_t innerFaceCount = 0;
    if(divsX >= 2 && divsY >= 2) {
        innerFaceCount = (divsX - 1) * (divsY - 1);
    }
    boundaryFaces.reserve((divsX + 1) * (divsY + 1) - innerFaceCount);
    std::vector<Core::Face*> innerFaces = std::vector<Core::Face*>();
    innerFaces.reserve(innerFaceCount);

    // Create edges and faces.
    for(std::size_t x = 0; x <= divsX; ++x) {
        for(std::size_t y = 0; y <= divsY; ++y) {
            std::size_t idx = x * (divsY + 2) + y;

            Core::Edge* e0 = new Core::Edge();
            Core::MakeEdge(verts.at(idx), verts.at(idx + 1), e0);

            Core::Edge* e1 = new Core::Edge();
            Core::MakeEdge(verts.at(idx + 1), verts.at(idx + 1 + divsY + 2), e1);

            Core::Edge* e2 = new Core::Edge();
            Core::MakeEdge(verts.at(idx + 1 + divsY + 2), verts.at(idx + divsY + 2), e2);

            Core::Edge* e3 = new Core::Edge();
            Core::MakeEdge(verts.at(idx + divsY + 2), verts.at(idx), e3);

            std::vector<Core::Edge*> loopEdges = {e0, e1, e2, e3};
            std::vector<Core::Vert*> loopVerts = {
                verts.at(idx), verts.at(idx + 1), verts.at(idx + 1 + divsY + 2), verts.at(idx + divsY + 2)};

            Core::Loop* newl = new Core::Loop();
            Core::MakeLoop(loopEdges, loopVerts, newl);
            Core::Face* newf = new Core::Face();
            Core::MakeFace(std::vector<Core::Loop*> {newl}, newf);

            // TODO: push faces, edges to appropriate vectors...
            bool isBoundaryFace = false;
            if(x == 0) {
                boundaryEdges.push_back(e0);
                isBoundaryFace = true;
            } else {
                innerEdges.push_back(e0);
            }
            if(x == divsX + 1) {
                boundaryEdges.push_back(e2);
                isBoundaryFace = true;
            } else {
                innerEdges.push_back(e2);
            }
            if(y == 0) {
                boundaryEdges.push_back(e3);
                isBoundaryFace = true;
            } else {
                innerEdges.push_back(e3);
            }
            if(x == divsY + 1) {
                boundaryEdges.push_back(e1);
                isBoundaryFace = true;
            } else {
                innerEdges.push_back(e1);
            }
            if(isBoundaryFace) {
                boundaryFaces.push_back(newf);
            } else {
                innerFaces.push_back(newf);
            }
        }
    }

    CreateGridResult result = CreateGridResult();
    result.innerEdges = innerEdges;
    result.innerFaces = innerFaces;
    result.innerVerts = innerVerts;
    result.outerEdges = boundaryEdges;
    result.outerFaces = boundaryFaces;
    result.outerVerts = boundaryVerts;

    return result;
}

} // namespace Ops
} // namespace Aoba
