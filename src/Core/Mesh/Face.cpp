#include "AobaAPI/Core/Mesh/Face.hpp"

#include "AobaAPI/Core/Mesh/Edge.hpp"
#include "AobaAPI/Core/Mesh/Loop.hpp"
#include "AobaAPI/Core/Mesh/Vert.hpp"

#include <algorithm>

namespace Aoba {
namespace Core {

Face::Face() {
    l = nullptr;
    m = nullptr;
    mNext = nullptr;
    mPrev = nullptr;
    index = 0;
    flags = 0;
    flagsIntern = 0;
    materialIdx = 0;
}

float Face::CalcArea() const { // TODO:
    // low priority
    // possible solution is to triangulate the face, and sum the areas of all triangles.
    return 0;
}

Math::Vec3 Face::CalcCenterAverage() const {
    std::vector<Vert*> faceVerts = Verts();
    Math::Vec3 result = Math::Vec3();
    for(int i = 0; i < faceVerts.size(); i++) {
        result += faceVerts.at(i)->co;
    }
    result /= float(faceVerts.size());
    return result;
}

float Face::CalcPerimiter() const {
    float result = 0;
    std::vector<Edge*> faceEdges = Edges();
    for(int i = 0; i < faceEdges.size(); i++) {
        result += faceEdges.at(i)->CalcLength();
    }
    return result;
}

void Face::NormalFlip() { // TODO:
    Loop* currentLoop = l;
    do {
        if(currentLoop->v == currentLoop->e->v1) {
            currentLoop->v = currentLoop->e->v2;
        } else {
            currentLoop->v = currentLoop->e->v1;
        }
        Loop* toModify = currentLoop;
        currentLoop = currentLoop->fNext;
        std::swap(toModify->fNext, toModify->fPrev);
    } while(currentLoop != l);
}

void Face::NormalUpdate() { // TODO:
    // low priority
}

const std::vector<Edge*> Face::Edges() const {
    std::vector<Edge*> result = std::vector<Edge*>();
    // iterate over all loops in face
    Loop* currentLoop = l;
    do {
        if(std::find(result.begin(), result.end(), currentLoop->e) == result.end()) { // check if already added
            result.push_back(currentLoop->e);
        }
        currentLoop = currentLoop->fNext;
    } while(currentLoop != l);
    return result;
}

const std::vector<Vert*> Face::Verts() const {
    std::vector<Vert*> result = std::vector<Vert*>();
    // iterate over all loops in current list
    Loop* currentLoop = l;
    do {
        if(std::find(result.begin(), result.end(), currentLoop->v) == result.end()) { // check if already added
            result.push_back(currentLoop->v);
        }
        currentLoop = currentLoop->fNext;
    } while(currentLoop != l);
    return result;
}

const std::vector<Loop*> Face::Loops() const {
    std::vector<Loop*> result = std::vector<Loop*>();
    Loop* current = l;

    result.push_back(current);
    while(current->fNext != l) {
        current = current->fNext;
        result.push_back(current);
    }

    return result;
}

} // namespace Core
} // namespace Aoba