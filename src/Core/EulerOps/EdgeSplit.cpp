#include "AobaAPI/Core/EulerOps.hpp"
#include "AobaAPI/Core/Mesh.hpp"

namespace Aoba {
namespace Core {

void EdgeSplit(Mesh* m, Edge* e, Vert* v, Edge* newe, Vert* newv) {
    newe->m = e->m;
    newv->m = e->m; // v can be nullptr
    // add the new edge between the appropriate verts
    // if v is not specified, add the new edge around v1.

    if(v == nullptr || v == e->v1) {
        // replace e with newe in list of verts around v1
        if(e->v1Prev == e && e->v1Next == e) {
            // e is the only edge around e->v1, handle this case differently
            e->v1->e = newe;
            newe->v1Next = newe;
            newe->v1Prev = newe;
            newv->e = e;
            // edges around e->v2 are unchanged
        } else {
            newv->e = newe;
            // e is not the only edge around e->v1
            newe->v1Next = e->v1Next;
            newe->v1Prev = e->v1Prev;
            if(e->v1->e == e) {
                e->v1->e = newe;
            }

            if(newe->v1Next->v1 == e->v1) {
                newe->v1Next->v1Prev = newe;
            } else {
                newe->v1Next->v2Prev = newe;
            }

            if(newe->v1Prev->v1 == e->v1) {
                newe->v1Prev->v1Next = newe;
            } else {
                newe->v1Prev->v2Next = newe;
            }
        }

        // list of edges around newv
        e->v1Next = newe;
        e->v1Prev = newe;
        newe->v2Next = e;
        newe->v2Prev = e;

        // loops
        if(e->l != nullptr) {
            Loop* current = e->l;
            std::vector<Loop*> newLoops = std::vector<Loop*>();
            do {
                Loop* newl = m->loopPool.Allocate();
                newl->f = current->f;
                newl->e = newe;
                newl->m = e->m;
                if(current->v == e->v1) {
                    // change loop vert
                    current->v = newv;
                    // add new loop as fPrev, with v in e->v1 and e in newe
                    newl->v = e->v1;
                    newl->fNext = current;
                    newl->fPrev = current->fPrev;
                    current->fPrev->fNext = newl;
                    current->fPrev = newl;
                } else {
                    // add new loop as fNext, with v in newv and e in newe
                    newl->v = newv;
                    newl->fPrev = current;
                    newl->fNext = current->fNext;
                    current->fNext->fPrev = newl;
                    current->fNext = newl;
                }
                newLoops.push_back(newl);

                current = current->eNext;
            } while(current != e->l);
            for(std::size_t i = 1; i < newLoops.size(); ++i) {
                newLoops.at(i)->ePrev = newLoops.at(i - 1);
                newLoops.at(i - 1)->eNext = newLoops.at(i);
            }
            newLoops.at(0)->ePrev = newLoops.back();
            newLoops.back()->eNext = newLoops.at(0);
            newe->l = newLoops.at(0);
        }

        // set verts around edges
        newe->v1 = e->v1;
        newe->v2 = newv;
        e->v1 = newv;

    } else {
        // replace e with newe in list of verts around v2
        if(e->v2Prev == e && e->v2Next == e) {
            // e is the only edge around e->v2, handle this case differently
            e->v2->e = newe;
            newe->v2Next = newe;
            newe->v2Prev = newe;
            newv->e = e;
            // edges around e->v1 are unchanged
        } else {
            newv->e = newe;
            // e is not the only edge around e->v2
            newe->v2Next = e->v2Next;
            newe->v2Prev = e->v2Prev;
            if(e->v2->e == e) {
                e->v2->e = newe;
            }

            if(newe->v2Next->v2 == e->v2) {
                newe->v2Next->v2Prev = newe;
            } else {
                newe->v2Next->v1Prev = newe;
            }

            if(newe->v2Prev->v2 == e->v2) {
                newe->v2Prev->v2Next = newe;
            } else {
                newe->v2Prev->v1Next = newe;
            }
        }

        // list of edges around newv
        e->v2Next = newe;
        e->v2Prev = newe;
        newe->v1Next = e;
        newe->v1Prev = e;

        // loops
        if(e->l != nullptr) {
            Loop* current = e->l;
            std::vector<Loop*> newLoops = std::vector<Loop*>();
            do {
                Loop* newl = m->loopPool.Allocate();
                newl->f = current->f;
                newl->e = newe;
                newl->m = e->m;
                if(current->v == e->v2) {
                    // change loop vert
                    current->v = newv;
                    // add new loop as fPrev, with v in e->v1 and e in newe
                    newl->v = e->v2;
                    newl->fNext = current;
                    newl->fPrev = current->fPrev;
                    current->fPrev->fNext = newl;
                    current->fPrev = newl;
                } else {
                    // add new loop as fNext, with v in newv and e in newe
                    newl->v = newv;
                    newl->fPrev = current;
                    newl->fNext = current->fNext;
                    current->fNext->fPrev = newl;
                    current->fNext = newl;
                }
                newLoops.push_back(newl);

                current = current->eNext;
            } while(current != e->l);
            for(std::size_t i = 1; i < newLoops.size(); ++i) {
                newLoops.at(i)->ePrev = newLoops.at(i - 1);
                newLoops.at(i - 1)->eNext = newLoops.at(i);
            }
            newLoops.at(0)->ePrev = newLoops.back();
            newLoops.back()->eNext = newLoops.at(0);
            newe->l = newLoops.at(0);
        }

        // set verts around edges
        newe->v2 = e->v2;
        newe->v1 = newv;
        e->v2 = newv;
    }

    // add newe, newv to mesh...
    m->edges->mPrev->mNext = newe;
    newe->mPrev = m->edges->mPrev;
    m->edges->mPrev = newe;
    newe->mNext = m->edges;
    m->edges = newe;

    m->verts->mPrev->mNext = newv;
    newv->mPrev = m->verts->mPrev;
    m->verts->mPrev = newv;
    newv->mNext = m->verts;
    m->verts = newv;
}

} // namespace Core
} // namespace Aoba