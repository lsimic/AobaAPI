#include "AobaAPI/Core/Mesh/Mesh.hpp"

#include "AobaAPI/Core/Mesh/Edge.hpp"
#include "AobaAPI/Core/Mesh/Face.hpp"
#include "AobaAPI/Core/Mesh/Loop.hpp"
#include "AobaAPI/Core/Mesh/Vert.hpp"
#include "AobaAPI/Math/Matrix/Matrix3.hpp"

namespace Aoba {
namespace Core {

Mesh::Mesh() {
    edges = nullptr;
    verts = nullptr;
    faces = nullptr;
}

bool Mesh::IsValid() const {
    // validate verts
    Vert* currentVert = verts;
    if(verts != nullptr) {
        do {
            // check that mesh pointer is correct
            if(currentVert->m != this) {
                return false;
            }
            if(currentVert->e != nullptr) {
                // check that first edge in list points to vert
                if(currentVert->e->v1 != currentVert && currentVert->e->v2 != currentVert) {
                    return false;
                }
                // check that first edge in list points to mesh
                if(currentVert->e->m != this) {
                    return false;
                }
            }
            // check that mNext, mPrev point to a vert in mesh
            if(currentVert->mPrev->mNext != currentVert || currentVert->mNext->mPrev != currentVert) {
                return false;
            }

            currentVert = currentVert->mNext;

        } while(verts != currentVert);
    }

    // validate edges
    Edge* currentEdge = edges;
    if(currentEdge != nullptr) {
        do {
            // check for self loops
            if(currentEdge->v1 == currentEdge->v2) {
                return false;
            }
            // TODO: check for double edges
            // Can do this in O(n) if using flags, if not, it's worse.

            if(currentEdge->l != nullptr) {
                Loop* currentLoop = currentEdge->l;
                // check that loop points to mesh
                if(currentLoop->m != this) {
                    return false;
                }
                // check ordering around verts, vNext/vPrev
                do {
                    if(currentLoop->e != currentEdge) {
                        return false;
                    }
                    if(currentLoop->ePrev->eNext != currentLoop || currentLoop->eNext->ePrev != currentLoop) {
                        return false;
                    }
                    currentLoop = currentLoop->eNext;
                } while(currentLoop != currentEdge->l);
            }

            // check that v1, v2 point to mesh
            if(currentEdge->v1->m != this || currentEdge->v2->m != this) {
                return false;
            }

            // check that mesh pointer is correct
            if(currentEdge->m != this) {
                return false;
            }
            // check that mNext, mPrev point to an edge in mesh
            if(currentEdge->mPrev->mNext != currentEdge || currentEdge->mNext->mPrev != currentEdge) {
                return false;
            }

            currentEdge = currentEdge->mNext;
        } while(edges != currentEdge);
    }

    // validate face and face loops
    if(faces != nullptr) {
        Face* currentFace = faces;
        do {
            // check that mesh pointer is correct
            if(currentFace->m != this) {
                return false;
            }
            // validate face loops
            Loop* currentLoop = currentFace->l;
            do {
                // check that loop edge, vert point to proper mesh
                if(currentLoop->e->m != this) {
                    return false;
                }
                if(currentLoop->v->m != this) {
                    return false;
                }
                // check that loop fNext, fPrev are correct
                if(currentLoop->fPrev->fNext != currentLoop || currentLoop->fNext->fPrev != currentLoop) {
                    return false;
                }
                // check that loop mesh pointer is correct
                if(currentLoop->m != this) {
                    return false;
                }

                currentLoop = currentLoop->fNext;
            } while(currentLoop->fNext != currentFace->l);

            // check that mNext, mPrev point to a face in mesh
            if(currentFace->m != this) {
                return false;
            }
            // TODO: check for double faces.

            currentFace = currentFace->mNext;
        } while(faces != currentFace);
    }

    // TODO: verify that each vert/edge/face appears exactly once in the vert/edge/face lists
    // this is very expensive without flags.

    return true;
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

const std::vector<Vert*> Mesh::Verts(std::function<bool(const Vert* const)> func) const {
    std::vector<Vert*> result = std::vector<Vert*>();
    if(verts == nullptr) {
        return result;
    }
    Vert* currentVert = verts;
    do {
        if(func(currentVert)) {
            result.push_back(currentVert);
        }
        currentVert = currentVert->mNext;
    } while(verts != currentVert);
    return result;
}

const std::vector<Edge*> Mesh::Edges(std::function<bool(const Edge* const)> func) const {
    std::vector<Edge*> result = std::vector<Edge*>();
    if(edges == nullptr) {
        return result;
    }
    Edge* currentEdge = edges;
    do {
        if(func(currentEdge)) {
            result.push_back(currentEdge);
        }
        currentEdge = currentEdge->mNext;
    } while(edges != currentEdge);
    return result;
}

const std::vector<Face*> Mesh::Faces(std::function<bool(const Face* const)> func) const {
    std::vector<Face*> result = std::vector<Face*>();
    if(faces == nullptr) {
        return result;
    }
    Face* currentFace = faces;
    do {
        if(func(currentFace)) {
            result.push_back(currentFace);
        }
        currentFace = currentFace->mNext;
    } while(faces != currentFace);
    return result;
}

} // namespace Core
} // namespace Aoba