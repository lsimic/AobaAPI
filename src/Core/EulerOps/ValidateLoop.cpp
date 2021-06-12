#include "AobaAPI/Core/EulerOps.hpp"
#include "AobaAPI/Core/Mesh.hpp"

#include <stdexcept>

namespace Aoba {
namespace Core {

bool ValidateLoop(std::vector<Edge*> edges, std::vector<Vert*> verts) {
    // faces must have at least three edges
    if(edges.size() < 3 || verts.size() < 3) {
        return false;
    }

    // verts and edges must be of same size
    if(edges.size() != verts.size()) {
        return false;
    }

    // edges and verts must form a continous closed loop.
    std::size_t vertIdx = 1;
    for(Edge* e : edges) {
        if(vertIdx == verts.size()) {
            if(verts.at(0) != e->Other(verts.back())) {
                return false;
            }
        } else {
            if(verts.at(vertIdx - 1) != e->Other(verts.at(vertIdx))) {
                return false;
            }
            vertIdx++;
        }
    }

    // check for duplicate verts
    // no need to explicitly check edges as well
    // if an edge is used twice, a vert must have been used twice as well.
    // this could be done in O(n) if i used flags to do it. but i don't want to use flags in eulerops
    for(std::size_t i = 0; i < verts.size(); ++i) {
        for(std::size_t j = 1; j < verts.size(); ++j) {
            if(verts.at(i) == verts.at(j)) {
                return false;
            }
        }
    }

    // seems ok 
    return true;
}

} // namespace Core
} // namespace Aoba