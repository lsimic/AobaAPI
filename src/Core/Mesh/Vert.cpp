#include "AobaAPI/Core/Mesh/Vert.hpp"

#include "AobaAPI/Core/Mesh/Edge.hpp"
#include "AobaAPI/Core/Mesh/Loop.hpp"

#include <algorithm>
#include <cmath>

namespace Aoba {
namespace Core {

Vert::Vert() {
    index = 0;
    flags = 0;
    flagsIntern = 0;
    e = nullptr;
    m = nullptr;
    mNext = nullptr;
    mPrev = nullptr;
}

bool Vert::IsBoundary() const {
    if(this->e == nullptr) {
        return false; // Isolated vert
    }
    Edge* currentEdge = this->e;
    do {
        // Check if current edge is boundary
        if(currentEdge->IsBoundary()) {
            return true; // boundary edge found
        }
        currentEdge = currentEdge->Next(this);
    } while(this->e != currentEdge);
    return false; // No boundary edges found
}

bool Vert::IsManifold() const {
    if(this->e == nullptr) {
        return false; // Isolated vert
    }
    // loop over all edges
    int edgeCount = 0;
    int boundaryEdgeCount = 0;
    Edge* currentEdge = this->e;
    Edge* lastBoundary = nullptr;
    do {
        // Check if current edge is wire
        if(!currentEdge->IsManifold()) {
            return false; // Non manifold edge found
        }
        if(currentEdge->IsBoundary()) {
            boundaryEdgeCount++;
            lastBoundary = currentEdge;
        }
        edgeCount++;
        currentEdge = currentEdge->Next(this);
    } while(this->e != currentEdge);
    // handle condition when there are boundary edges
    if(boundaryEdgeCount > 0) {
        if(boundaryEdgeCount > 2) {
            return false; // boundary verts always have two adjecent boundary edges.
        }
        // try traversing all edges using loops starting from one boundary edge and stopping on other.
        currentEdge = lastBoundary;
        Loop* currentLoop = currentEdge->l;
        int traversedEdgeCount = 1;
        if(currentLoop->v == this) {
            do {
                currentLoop = currentLoop->fPrev->eNext; // always two loops, already checked for manifold
                traversedEdgeCount++;
            } while(!currentLoop->e->IsBoundary());
        } else {
            do {
                currentLoop = currentLoop->fNext->eNext; // always two loops, already checked for manifold
                traversedEdgeCount++;
            } while(!currentLoop->e->IsBoundary());
        }
        if(traversedEdgeCount < edgeCount) {
            return false; // unable to traverse all edges using loops
        } else {
            return true; // traversed all edges using loops
        }
    }

    // no boundary edges, should be able to traverse all edges and end up where started.
    Loop* currentLoop = this->e->l;
    int traversedEdgeCount = 1;
    if(currentLoop->v == this) {
        do {
            currentLoop = currentLoop->fPrev->eNext; // always two loops, already checked for manifold
            traversedEdgeCount++;
        } while(currentLoop->e != this->e);
    } else {
        do {
            currentLoop = currentLoop->fNext->eNext; // always two loops, already checked for manifold
            traversedEdgeCount++;
        } while(currentLoop->e != this->e);
    }
    if(traversedEdgeCount < edgeCount) {
        return false; // unable to traverse all edges using loops
    } else {
        return true; // traversed all edges using loops
    }
}

bool Vert::IsWire() const {
    if(this->e == nullptr) {
        return false; // Isolated vert
    }
    Edge* currentEdge = this->e;
    do {
        // Check if current edge is wire
        if(currentEdge->IsWire()) {
            return true; // wire edge found
        }
        currentEdge = currentEdge->Next(this);
    } while(this->e != currentEdge);
    return false; // No wire edges found
}

const std::vector<Edge*> Vert::Edges() const {
    // in case of self edges(e.v1 == e.v2) v1Next is equal to v2Next, and v1Prev is equal to v2Prev.
    // no need to handle this condition separately
    std::vector<Edge*> result = std::vector<Edge*>();
    if(this->e == nullptr) {
        return result; // no adjecent edges.
    }
    Edge* currentEdge = this->e;
    result.push_back(currentEdge);
    while(this->e != currentEdge->Next(this)) {
        currentEdge = currentEdge->Next(this);
        result.push_back(currentEdge);
    }
    return result;
}

const std::vector<Face*> Vert::Faces() const {
    std::vector<Face*> result = std::vector<Face*>();
    if(this->e == nullptr) {
        return result;
    }
    // loop over all edges of this vert
    Edge* currentEdge = this->e;
    do {
        // loop over all loops of the current edge
        if(currentEdge->l != nullptr) {
            Loop* currentLoop = currentEdge->l;
            do {
                // if the loop's face not already in the list, add it to the list
                if(std::find(result.begin(), result.end(), currentLoop->f) == result.end()) {
                    result.push_back(currentLoop->f);
                }
            } while(currentEdge->l != currentLoop);
        }
        currentEdge = currentEdge->Next(this);
    } while(this->e != currentEdge);
    return result;
}

} // namespace Core
} // namespace Aoba
