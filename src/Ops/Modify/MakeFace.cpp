#include "AobaAPI/Ops/Modify.hpp"

#include <stdexcept>

namespace Aoba {
namespace Ops {

Core::Face* MakeFace(Core::Mesh* m, std::vector<Core::Edge*> edges) {

    if(edges.size() < 3) {
        throw std::invalid_argument("Face must have at least 3 distinct edges.");
    }

    // arrays to hold ordered edges and verts.
    std::vector<Core::Edge*> loopEdges = std::vector<Core::Edge*>();
    std::vector<Core::Vert*> loopVerts = std::vector<Core::Vert*>();

    loopEdges.push_back(edges.at(0));
    if(edges.at(0)->IsBoundary()) {
        if(edges.at(0)->Loops().at(0)->LoopVert() == edges.at(0)->V1()) {
            loopVerts.push_back(edges.at(0)->V2());
            loopVerts.push_back(edges.at(0)->V1());
        } else {
            loopVerts.push_back(edges.at(0)->V1());
            loopVerts.push_back(edges.at(0)->V2());
        }
    } else {
        loopVerts.push_back(edges.at(0)->V1());
        loopVerts.push_back(edges.at(0)->V2());
    }

    edges.erase(edges.begin()); // remove first edge from input edges

    // find edge which contains last vert in the loop
    // push that edge into the loop
    // add another vert into the loop which is the other vert of the found edge
    while(!edges.empty()) {
        bool found = false;
        for(auto it = edges.begin(); it != edges.end(); ++it) {
            if((*it)->V1() == loopVerts.back()) {
                loopEdges.push_back((*it));
                loopVerts.push_back((*it)->V2());
                found = true;
                edges.erase(it);
                break;
            } else if((*it)->V2() == loopVerts.back()) {
                loopEdges.push_back((*it));
                loopVerts.push_back((*it)->V1());
                found = true;
                edges.erase(it);
                break;
            }
        }
        if(!found) {
            throw std::invalid_argument("Specified edges do not form a true circuit");
        }
    }
    if(loopVerts.at(0) != loopVerts.back()) {
        throw std::invalid_argument("Specified edges do not form a true circuit");
    }
    loopVerts.pop_back();

    // use flags to check for duplicates
    bool duplicate = false;
    int32_t MARKED = 1 << 0;
    for(Core::Vert* v : loopVerts) {
        if(v->flagsIntern & MARKED) {
            duplicate = true;
            break;
        }
        v->flagsIntern = MARKED;
    }
    for(Core::Vert* v : loopVerts) {
        v->flagsIntern = 0;
    }

    if(duplicate) {
        throw std::invalid_argument("Duplicate verts found");
    }

    Core::Face* newf = m->facePool.Allocate();
    std::vector<Core::Loop*> loops = m->loopPool.Allocate(loopEdges.size());
    Core::MakeLoop(m, loopEdges, loopVerts, loops);
    Core::MakeFace(m, loops.at(0), newf);
    return newf;
}

} // namespace Ops
} // namespace Aoba
