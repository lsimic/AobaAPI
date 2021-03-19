#include "AobaAPI/Core/EulerOps.hpp"
#include "AobaAPI/Core/Mesh.hpp"

#include <stdexcept>

namespace Aoba {
namespace Core {

void MakeFace(std::vector<Edge*> edges, Face* newf) {
    Mesh* m = edges.at(0)->m;

    // arrays to hold ordered edges and verts.
    std::vector<Edge*> loopEdges = std::vector<Edge*>();
    std::vector<Vert*> loopVerts = std::vector<Vert*>();

    loopEdges.push_back(edges.at(0));
    loopVerts.push_back(edges.at(0)->v1);
    loopVerts.push_back(edges.at(0)->v2);

    edges.erase(edges.begin()); // remove first edge from input edges?

    // find edge which contains last vert in the loop
    // push that edge into the loop
    // add another vert into the loop which is the other vert of the found edge
    while(!edges.empty()) {
        bool found = false;
        for(auto it = edges.begin(); it != edges.end(); ++it) {
            if((*it)->v1 == loopVerts.back()) {
                loopEdges.push_back((*it));
                loopVerts.push_back((*it)->v2);
                found = true;
                edges.erase(it);
                break;
            } else if((*it)->v2 == loopVerts.back()) {
                loopEdges.push_back((*it));
                loopVerts.push_back((*it)->v1);
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

    // TODO: do i need to explicitly check for some strange degenerate faces
    // think two rectangles touching upon a vertex etc
    // which might be traversable by the above loop in some cases
    // but might not form a valid face.
    // think - creating a face where an edge is used in multiple orientations
    // how to handle cases such as that one?

    // loopEdges and loopVerts should now contain ordered elements to create loops
    // if the edges specified were invalid, an arror should have thrown

    // create the face and a single facelooplist.

    std::vector<Loop*> loops = std::vector<Loop*>();
    // create loop for each edge/vert pair
    for(int i = 0; i < loopEdges.size(); ++i) {
        Loop* newl = new Loop();
        newl->v = loopVerts.at(i);
        newl->e = loopEdges.at(i);
        newl->f = newf;
        newl->m = loopEdges.at(i)->m;

        // no existing loops using edge
        Edge* currentEdge = loopEdges.at(i);
        if(currentEdge->l == nullptr) {
            newl->eNext = newl;
            newl->ePrev = newl;
            currentEdge->l = newl;
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

        loops.push_back(newl);
    }

    // loops are now created, must specify their face ordering.
    for(std::size_t i = 1; i < loops.size() - 1; ++i) {
        loops.at(i)->fNext = loops.at(i + 1);
        loops.at(i)->fPrev = loops.at(i - 1);
    }
    loops.at(0)->fNext = loops.at(1);
    loops.at(0)->fPrev = loops.back();
    loops.back()->fNext = loops.at(0);
    loops.back()->fPrev = loops.at(loops.size() - 2);

    // loops now complete, finalize the face by creating faceLoopList and all that
    FaceLoopList* flList = new FaceLoopList();
    flList->first = loops.at(0);
    flList->next = flList;
    flList->prev = flList;

    newf->loops = flList;
    newf->m = loops.at(0)->m;

    // add newf to mesh
    if(m->faces == nullptr) {
        m->faces = newf;
        newf->mNext = newf;
        newf->mPrev = newf;
    } else {
        m->faces->mPrev->mNext = newf;
        newf->mPrev = m->faces->mPrev;
        m->faces->mPrev = newf;
        newf->mNext = m->faces;
        m->faces = newf;
    }
}

void MakeFace(std::vector<Loop*> loops, Face* newf) {
    Mesh* m = loops.at(0)->m;
    std::vector<FaceLoopList*> fllists = std::vector<FaceLoopList*>();
    fllists.reserve(loops.size());

    // Create facelooplists for each input loop, point the face to the first item in list
    for(std::size_t i = 0; i < loops.size(); ++i) {
        FaceLoopList* newfll = new FaceLoopList();
        newfll->first = loops.at(i);
        fllists.push_back(newfll);
    }
    for(std::size_t i = 1; i < fllists.size(); i++) {
        fllists.at(i)->prev = fllists.at(i - 1);
        fllists.at(i - 1)->next = fllists.at(i);
    }
    fllists.at(0)->prev = fllists.back();
    fllists.back()->next = fllists.at(0);

    // link the loops with the new face.
    newf->loops = fllists.at(0);
    for(std::size_t i = 1; i < fllists.size(); i++) {
        Loop* current = fllists.at(i)->first;

        current->f = newf;
        while(current->fNext != fllists.at(i)->first) {
            current->f = newf;
        }
    }

    // add newf to mesh
    if(m->faces == nullptr) {
        m->faces = newf;
        newf->mNext = newf;
        newf->mPrev = newf;
    } else {
        m->faces->mPrev->mNext = newf;
        newf->mPrev = m->faces->mPrev;
        m->faces->mPrev = newf;
        newf->mNext = m->faces;
        m->faces = newf;
    }

    // link face to mesh
    newf->m = loops.at(0)->m;
}

} // namespace Core
} // namespace Aoba