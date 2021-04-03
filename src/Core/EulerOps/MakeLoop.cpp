#include "AobaAPI/Core/EulerOps.hpp"
#include "AobaAPI/Core/Mesh.hpp"

#include <stdexcept>

namespace Aoba {
namespace Core {

void MakeLoop(std::vector<Edge*> edges, std::vector<Vert*> verts, Loop* first) {
    std::vector<Loop*> newLoops = std::vector<Loop*>();
    newLoops.reserve(edges.size());

    newLoops.push_back(first);
    for(std::size_t i = 0; i < edges.size() - 1; i++) {
        newLoops.push_back(new Loop());
    }

    // TODO: implement loop verification here?

    // create loops for each vert-edge pair
    for(std::size_t i = 0; i < edges.size(); ++i) {
        Edge* currentEdge = edges.at(i);

        // create the new loop
        Loop* newl = newLoops.at(i);
        // TODO: check wether vert is adjecent to edge
        newl->e = currentEdge;
        newl->v = verts.at(i);
        newl->m = currentEdge->m;

        // add the new loop to edge
        // no existing loops using edge
        if(currentEdge->l == nullptr) {
            currentEdge->l = newl;
            newl->eNext = newl;
            newl->ePrev = newl;
        } else {
            // edge already has some adjecent loops
            Loop* currentLoop = currentEdge->l;
            Loop* prevLoop = currentLoop->ePrev;
            currentEdge->l = newl;

            newl->ePrev = prevLoop;
            newl->eNext = currentLoop;
            currentLoop->ePrev = newl;
            prevLoop->eNext = newl;
        }
    }

    for(std::size_t i = 1; i < newLoops.size(); i++) {
        newLoops.at(i)->fPrev = newLoops.at(i - 1);
        newLoops.at(i - 1)->fNext = newLoops.at(i);
    }
    newLoops.at(0)->fPrev = newLoops.back();
    newLoops.back()->fNext = newLoops.at(0);
}

} // namespace Core
} // namespace Aoba