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

;Math::Vec3 Edge::CalcLocalNormal(Loop* loop) const {
    // use v1, v2, loop->fPrev.v
    Math::Vec3 result = Math::Vec3();

    Loop* current = loop->fPrev;
    do {
        Math::Vec3& vc = current->v->co;
        Math::Vec3& vn = current->fNext->v->co;
        result.x += (vc.y - vn.y) * (vc.z + vn.z);
        result.y += (vc.z - vn.z) * (vc.x + vn.x);
        result.z += (vc.x - vn.x) * (vc.y + vn.y);
        current = current->fNext;
    } while(current != loop->fNext->fNext);
    //if face is not triangular, additional iteration to make a quad
    if(loop->fNext->fNext != loop->fPrev) {
        Math::Vec3& vc = loop->fNext->fNext->v->co;
        Math::Vec3& vn = loop->fPrev->v->co;
        result.x += (vc.y - vn.y) * (vc.z + vn.z);
        result.y += (vc.z - vn.z) * (vc.x + vn.x);
        result.z += (vc.x - vn.x) * (vc.y + vn.y);
    }

    result.Normalize();
    return result;
}

float Edge::CalcFaceAngle() const { 
    // check if edge has exactly two faces
    if(l == nullptr || l->eNext != l || l->eNext != l->ePrev) {
        throw std::invalid_argument("Edge must have exactly two faces");
    }

    // calculate nearby normals for this edge. Usefull if face is not flat
    Math::Vec3 no1 = CalcLocalNormal(l);
    Math::Vec3 no2 = CalcLocalNormal(l->eNext);
    return no1.Angle(no2);
}

float Edge::CalcFaceAngleSigned() const { 
    // check if edge has exactly two faces
    if(l == nullptr || l->eNext != l || l->eNext != l->ePrev) {
        throw std::invalid_argument("Edge must have exactly two faces");
    }

    // calculate nearby normals for this edge. Usefull if face is not flat
    Math::Vec3 no1 = CalcLocalNormal(l);
    Math::Vec3 no2 = CalcLocalNormal(l->eNext);

    Math::Vec3 cross = no1.Cross(no2);
    Math::Vec3 dir = l->fNext->v->co - l->v->co;

    if(dir.Dot(cross) > 0.0f) {
        return no1.Angle(no2);
    } else {
        return -(no1.Angle(no2));
    }
}

float Edge::CalcLength() const {
    return (this->v1->co - this->v2->co).Magnitude();
}

Vert* Edge::Other(const Vert* v) const {
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
        // check if the face already added
        // handles edges which are used by the same face in multiple orientations
        if(std::find(result.begin(), result.end(), currentLoop->f) == result.end()) { 
            // face not present, add to result
            result.push_back(currentLoop->f);
        }
        currentLoop = currentLoop->eNext;
    } while(currentLoop != this->l);
    return result;
}

const std::vector<Loop*> Edge::Loops() const {
    std::vector<Loop*> result = std::vector<Loop*>();
    if(this->l == nullptr) {
        return result;
    }

    Loop* currentLoop = this->l;
    do {
        result.push_back(currentLoop);
        currentLoop = currentLoop->eNext;
    } while(currentLoop != this->l);
    return result;
}

const std::vector<Vert*> Edge::Verts() const {
    return std::vector<Vert*> {this->v1, this->v2};
}

const std::vector<Face*> Edge::Faces(std::function<bool(const Face* const)> func) const {
    std::vector<Face*> result = std::vector<Face*>();
    if(this->l == nullptr) {
        return result;
    }

    Loop* currentLoop = this->l;
    do {
        if(func(currentLoop->f)) {
            // check if the face already added
            // handles edges which are used by the same face in multiple orientations
            if(std::find(result.begin(), result.end(), currentLoop->f) == result.end()) {
                // face not present, add to result
                result.push_back(currentLoop->f);
            }
        }
        currentLoop = currentLoop->eNext;
    } while(currentLoop != this->l);
    return result;
}

const std::vector<Loop*> Edge::Loops(std::function<bool(const Loop* const)> func) const {
    std::vector<Loop*> result = std::vector<Loop*>();
    if(this->l == nullptr) {
        return result;
    }

    Loop* currentLoop = this->l;
    do {
        if(func(currentLoop)) {
            result.push_back(currentLoop);
        }
        currentLoop = currentLoop->eNext;
    } while(currentLoop != this->l);
    return result;
}

const std::vector<Vert*> Edge::Verts(std::function<bool(const Vert* const)> func) const {
    std::vector<Vert*> result = std::vector<Vert*>();

    if(func(v1)) {
        result.push_back(v1);
    }
    if(func(v2)) {
        result.push_back(v2);
    }
    return result;
}

Vert* Edge::V1() const {
    return v1;
}

Vert* Edge::V2() const {
    return v2;
}

} // namespace Core
} // namespace Aoba