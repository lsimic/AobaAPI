#include "AobaAPI/Core/Mesh/Mesh.hpp"
#include "AobaAPI/Core/Mesh/Face.hpp"
#include "AobaAPI/Core/Mesh/Edge.hpp"
#include "AobaAPI/Core/Mesh/Vert.hpp"
#include "AobaAPI/Math/Matrix/Matrix3.hpp"

namespace Aoba {
namespace Core {

Mesh::Mesh() {
    edges = nullptr;
    verts = nullptr;
    faces = nullptr;
}

bool Mesh::IsValid() const { // TODO: 
    return false;
}

void Mesh::Transform(Math::Mat4 mat) {
    if(verts != nullptr) {
        Math::Mat3 transformMatrix = Math::Mat3();
        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < 3; j++) {
                transformMatrix(i, j) = mat(i, j); 
            }
        }

        Vert* currentVert = verts;
        do {
            currentVert->co = transformMatrix * currentVert->co;
            currentVert->co.x += mat(0, 3);
            currentVert->co.y += mat(1, 3);
            currentVert->co.z += mat(2, 3);
            currentVert = currentVert->mNext;
        } while(verts != currentVert);
    }
}

const std::vector<Vert*> Mesh::Verts() const {
    std::vector<Vert*> result = std::vector<Vert*>();
    if(verts == nullptr) {
        return result;
    }
    Vert* currentVert = verts;
    do {
        result.push_back(currentVert);
        currentVert = currentVert->mNext;
    } while(verts != currentVert);
    return result;
}

const std::vector<Edge*> Mesh::Edges() const {
    std::vector<Edge*> result = std::vector<Edge*>();
    if(edges == nullptr) {
        return result;
    }
    Edge* currentEdge = edges;
    do {
        result.push_back(currentEdge);
        currentEdge = currentEdge->mNext;
    } while(edges != currentEdge);
    return result; 
}

const std::vector<Face*> Mesh::Faces() const {
    std::vector<Face*> result = std::vector<Face*>();
    if(faces == nullptr) {
        return result;
    } 
    Face* currentFace = faces;
    do {
        result.push_back(currentFace);
        currentFace = currentFace->mNext;
    } while(faces != currentFace);
    return result;
}

} // namespace Core
} // namespace Aoba