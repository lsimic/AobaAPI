#include "AobaAPI/Ops/Create.hpp"

namespace Aoba {
namespace Ops {

const CreateGridResult CreateGrid(Core::Mesh* m, int divisionsX, int divisionsY, float sizeX, float sizeY) {
    std::size_t divsX = divisionsX;
    std::size_t divsY = divisionsY;
    // Create verts.
    std::vector<Core::Vert*> verts = m->vertPool.Allocate((2 + divsX) * (2 + divsY));
    std::vector<Core::Vert*> boundaryVerts = std::vector<Core::Vert*>();
    boundaryVerts.reserve(divsX + 2 * (2 + divsX));
    std::vector<Core::Vert*> innerVerts = std::vector<Core::Vert*>();
    innerVerts.reserve(divsX * divsY);

    float xStep = sizeX / (divsX + 1);
    float yStep = sizeY / (divsY + 1);
    std::size_t vIdx = 0;
    for(int x = 0; x <= divsX + 1; ++x) {
        for(int y = 0; y <= divsY + 1; ++y) {
            Core::Vert* newv = verts.at(vIdx);
            Core::MakeVert(m, newv);
            newv->co = Math::Vec3(-sizeX / 2 + x * xStep, -sizeY / 2 + y * yStep, 0);
            ++vIdx; 
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

    // create edges paralel to x axi
    std::vector<Core::Edge*> xEdges = m->edgePool.Allocate((divsX + 1) * (divsY + 2));
    std::size_t eIdx = 0;
    for(std::size_t y = 0; y <= divsY + 1; ++y) {
        for(std::size_t x = 0; x <= divsX; ++x) {
            std::size_t idx = y * (2 + divsX) + x;
            Core::Edge* newe = xEdges.at(eIdx);
            ++eIdx; 
            Core::MakeEdge(m, verts.at(idx), verts.at(idx + 1), newe);
            if(y == 0 || y == divsY + 1) {
                boundaryEdges.push_back(newe);
            } else {
                innerEdges.push_back(newe);
            }
        }
    }

    // create edges paralel to y axis
    std::vector<Core::Edge*> yEdges = m->edgePool.Allocate((divsY + 1) * (divsX + 2));
    eIdx = 0;
    for(std::size_t y = 0; y <= divsY; ++y) {
        for(std::size_t x = 0; x <= divsX + 1; ++x) {
            std::size_t idx = y * (2 + divsX) + x;
            Core::Edge* newe = yEdges.at(eIdx);
            ++eIdx;
            Core::MakeEdge(m, verts.at(idx), verts.at(idx + 2 + divsX), newe);
            if(x == 0 || x == divsX + 1) {
                boundaryEdges.push_back(newe);
            } else {
                innerEdges.push_back(newe);
            }
        }
    }

    std::vector<Core::Face*> boundaryFaces = std::vector<Core::Face*>();
    std::size_t innerFaceCount = 0;
    if(divsX >= 2 && divsY >= 2) {
        innerFaceCount = (divsX - 1) * (divsY - 1);
    }
    boundaryFaces.reserve((divsX + 1) * (divsY + 1) - innerFaceCount);
    std::vector<Core::Face*> innerFaces = std::vector<Core::Face*>();
    innerFaces.reserve(innerFaceCount);


    std::vector<Core::Face*> faces = m->facePool.Allocate((divsX + 1) * (divsY + 1));
    std::vector<Core::Loop*> loops = m->loopPool.Allocate(faces.size() * 4);

    // Create edges and faces.
    std::size_t fIdx = 0;
    for(std::size_t x = 0; x <= divsX; ++x) {
        for(std::size_t y = 0; y <= divsY; ++y) {
            Core::Edge* e0 = xEdges.at(x + y * (1 + divsX));
            Core::Edge* e1 = yEdges.at(x + (2 + divsX) * y + 1);
            Core::Edge* e2 = xEdges.at(x + (1 + y) * (1 + divsX));
            Core::Edge* e3 = yEdges.at(x + (2 + divsX) * y);

            Core::Vert* v0 = verts.at(y * (2 + divsX) + x);
            Core::Vert* v1 = verts.at(y * (2 + divsX) + x + 1);
            Core::Vert* v2 = verts.at((1 + y) * (2 + divsX) + x + 1);
            Core::Vert* v3 = verts.at((1 + y) * (2 + divsX) + x);

            std::vector<Core::Edge*> loopEdges = {e0, e1, e2, e3};
            std::vector<Core::Vert*> loopVerts = {v0, v1, v2, v3};
            std::vector<Core::Loop*> faceLoops =
                std::vector<Core::Loop*>(loops.begin() + (4 * fIdx), loops.begin() + (4 * fIdx) + 4);
            Core::MakeLoop(m, loopEdges, loopVerts, faceLoops);

            Core::Face* newf = faces.at(fIdx);
            Core::MakeFace(m, faceLoops.at(0), newf);
            ++fIdx;
            // TODO: push faces, edges to appropriate vectors...
            bool isBoundaryFace = false;
            if(x == 0 || x == divsX) {
                isBoundaryFace = true;
            } else if(y == 0 || y == divsY) {
                isBoundaryFace = true;
            } else {
                isBoundaryFace = false;
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
