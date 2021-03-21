#include "AobaAPI/Core/Mesh/Face.hpp"

#include "AobaAPI/Core/Mesh/Edge.hpp"
#include "AobaAPI/Core/Mesh/Loop.hpp"
#include "AobaAPI/Core/Mesh/Vert.hpp"

#include <algorithm>

namespace Aoba {
namespace Core {

Face::Face() {
    loops = nullptr;
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
    // low priority
}

void Face::NormalUpdate() { // TODO:
    // low priority
}

const std::vector<Edge*> Face::Edges() const {
    std::vector<Edge*> result = std::vector<Edge*>();
    // iterate over all loop lists (all loops of the face)
    FaceLoopList* currentLoopList = this->loops;
    do {
        // iterate over all loops in current list
        Loop* currentLoop = currentLoopList->first;
        do {
            if(std::find(result.begin(), result.end(), currentLoop->e) == result.end()) { // check if already added
                result.push_back(currentLoop->e);
            }
            currentLoop = currentLoop->fNext;
        } while(currentLoop != currentLoopList->first);
        currentLoopList = currentLoopList->next;
    } while(currentLoopList != this->loops);
    return result;
}

const std::vector<Vert*> Face::Verts() const {
    std::vector<Vert*> result = std::vector<Vert*>();
    // iterate over all loop lists (all loops of the face)
    FaceLoopList* currentLoopList = this->loops;
    do {
        // iterate over all loops in current list
        Loop* currentLoop = currentLoopList->first;
        do {
            if(std::find(result.begin(), result.end(), currentLoop->v) == result.end()) { // check if already added
                result.push_back(currentLoop->v);
            }
            currentLoop = currentLoop->fNext;
        } while(currentLoop != currentLoopList->first);
        currentLoopList = currentLoopList->next;
    } while(currentLoopList != this->loops);
    return result;
}

const std::vector<FaceLoopList*> Face::LoopLists() const {
    std::vector<FaceLoopList*> result = std::vector<FaceLoopList*>();
    FaceLoopList* current = this->loops;

    result.push_back(current);
    while(current->next != this->loops) {
        current = current->next;
        result.push_back(current);
    }

    return result;
}

const std::vector<Loop*> FaceLoopList::Loops() const {
    std::vector<Loop*> result = std::vector<Loop*>();
    Loop* current = this->first;

    result.push_back(current);
    while(current->fNext != this->first) {
        current = current->fNext;
        result.push_back(current);
    }

    return result;
}

FaceLoopList::FaceLoopList() {
    next = nullptr;
    prev = nullptr;
    first = nullptr;
}

} // namespace Core
} // namespace Aoba