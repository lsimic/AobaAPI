#include "AobaAPI/Core/Mesh/Edge.hpp"

#include "AobaAPI/Core/Mesh/Loop.hpp"
#include "AobaAPI/Core/Mesh/Vert.hpp"

#include <algorithm>
#include <stdexcept>

namespace Aoba {
namespace Core {

Edge::Edge() {
    index = 0;
    flags = 0;
    flagsIntern = 0;
    v1 = nullptr;
    v2 = nullptr;
    l = nullptr;
    v1Next = nullptr;
    v1Prev = nullptr;
    v2Next = nullptr;
    v2Prev = nullptr;
    m = nullptr;
    mNext = nullptr;
    mPrev = nullptr;
}

Edge* Edge::Next(const Vert* v) const {
    if(v == this->v1) {
        return this->v1Next;
    }
    if(v == this->v2) {
        return this->v2Next;
    }
    throw std::invalid_argument("Specified vert not used by the edge.");
}

Edge* Edge::Prev(const Vert* v) const {
    if(v == this->v1) {
        return this->v1Prev;
    }
    if(v == this->v2) {
        return this->v2Prev;
    }
    throw std::invalid_argument("Specified vert not used by the edge.");
}

float Edge::CalcFaceAngle() const { // TODO:
    // check if adjecent to exactly two faces
    // use normals to calculate
    // low priority
    // how to handle non-flat faces?
    return 0;
}

float Edge::CalcFaceAngleSigned() const { // TODO:
    // check if adjecent to exactly two faces
    // use normals to calculate
    // low priority
    // how to handle non-flat faces?
    return 0;
}

float Edge::CalcLength() const {
    return (this->v1->co - this->v2->co).Magnitude();
}

const Vert* Edge::Other(const Vert* v) const {
    if(this->v1 == v) {
        return this->v2;
    } else if(this->v2 == v) {
        return this->v1;
    } else {
        throw std::invalid_argument("Specified vert not used by the edge.");
    }
}

bool Edge::IsBoundary() const {
    if(this->l == this->l->eNext && this->l == this->l->ePrev) {
        return true;
    }
    return false;
}

bool Edge::IsContigous() const {
    if(this->l == nullptr) {
        return true; // wire edge
    }
    if(this->l == this->l->eNext) {
        // not checking ePrev here. if it is not the same, mesh is in invalid state
        return true; // boundary edge
    }
    int count = 1;
    Loop* current = this->l;
    while(current->eNext != this->l) {
        count++;
        if(count > 2) {
            return false; // more than 2 loops per edge
            // there will always be two faces pointing in separate direction in this case.
        }
        current = current->eNext;
    }
    if(count == 2) {
        if(this->l->v == this->l->eNext->v) {
            return false; // adjecent faces pointing in separate direction
        }
    }
    return true;
}

bool Edge::IsManifold() const {
    if(this->l == nullptr) {
        return false; // wire edge
    }
    int count = 1;
    Loop* currentLoop = this->l;
    while(currentLoop->eNext != this->l) {
        count++;
        if(count > 2) {
            return false; // more than 2 loops per edge
        }
        currentLoop = currentLoop->eNext;
    }
    if(count == 2) {
        if(this->l->v == this->l->eNext->v) {
            return false; // adjecent faces pointing in separate direction
            // (if edge is used twice, it should be used in different orientations)
        }
    }
    return true;
}

bool Edge::IsWire() const {
    if(this->l == nullptr) {
        return true;
    }
    return false;
}

const std::vector<Face*> Edge::Faces() const {
    std::vector<Face*> result = std::vector<Face*>();
    if(this->l == nullptr) {
        return result;
    }

    Loop* currentLoop = this->l;
    do {
        if(std::find(result.begin(), result.end(), currentLoop->f) == result.end()) { // check if face already added
            // face not present, add to result
            result.push_back(currentLoop->f);
        }
        currentLoop = currentLoop->eNext;
    } while(currentLoop != this->l);
    return result;
}

const std::vector<Vert*> Edge::Verts() const {
    return std::vector<Vert*> {this->v1, this->v2};
}

} // namespace Core
} // namespace Aoba