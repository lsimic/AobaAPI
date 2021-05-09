#include "AobaAPI/Core/EulerOps.hpp"
#include "AobaAPI/Core/Mesh.hpp"

#include <algorithm>

namespace Aoba {
namespace Core {

void deleteMergeableFaces(Edge* v1e1, Edge* v1e2, Edge* v2e1, Edge* v2e2) {
    // check if v1e1, v1e2 share a face
    std::vector<Face*> facesv1 = std::vector<Face*>();
    for(Face* fe1 : v1e1->Faces()) {
        for(Face* fe2 : v1e2->Faces()) {
            if(fe1 == fe2) {
                facesv1.push_back(fe1);
            }
        }
    }
    if(facesv1.size() < 1) {
        return;
    }

    // check if v2e1, v2e2 share a face
    std::vector<Face*> facesv2 = std::vector<Face*>();
    for(Face* fe1 : v2e1->Faces()) {
        for(Face* fe2 : v2e2->Faces()) {
            if(fe1 == fe2) {
                facesv2.push_back(fe1);
            }
        }
    }
    if(facesv2.size() < 1) {
        return;
    }

    // both edges around v1 and both edges around v2 have a common face
    // for each face in facesv1, check if there are faces in facesf2 which match the rest of it's loop
    std::vector<Face*> facesToKill = std::vector<Face*>();

    for(Face* fv1 : facesv1) {
        std::vector<Edge*> fv1Edges = fv1->Edges();
        for(Face* fv2 : facesv2) {
            bool notFound = false;
            for(Edge* fv2Edge : fv2->Edges()) {
                if(fv2Edge != v2e1 && fv2Edge != v2e2) {
                    // ignoring edges aroud v2, v1
                    // checking edges here. if the faces are same, edges will be same, but opposite orientation
                    if(std::find(fv1Edges.begin(), fv1Edges.end(), fv2Edge) == fv1Edges.end()) {
                        // edge from face fv2 was not found in face fv1
                        notFound = true;
                        break;
                    }
                }
            }

            if(!notFound) {
                // found all edges of fv2 in fv1
                // mark fv2 for deletion
                facesToKill.push_back(fv2);
            }
        }
    }

    for(Face* f : facesToKill) {
        KillFace(f);
    }
}

// this code is... interesting
// several parts are rather costly, and some checks could be separated, at the cost of safety
void GlueVert(Vert* v1, Vert* v2) {
    std::vector<Edge*> v1Edges = v1->Edges();
    std::vector<Edge*> v2Edges = v2->Edges();

    // check if there is an edge between v1, v2
    Edge* common = nullptr;
    for(Edge* e : v1Edges) {
        if(e->Other(v1) == v2) {
            common = e;
            break;
        }
    }

    // TODO: in order to handle the case where v1 and v2 belong to the same face
    // but do not share an edge
    // call manifoldMakeEdge, to split the face
    // if the remaining faces are triangles, they will get dissolved.
    // not the ideal solution, but works
    if(common == nullptr) {
        // check if v1 and v2 are used by the same face...
        Face* found = nullptr;
        std::vector<Face*> v1Faces = v1->Faces();
        std::vector<Face*> v2Faces = v2->Faces();
        for(Face* f1 : v1Faces) {
            if(std::find(v2Faces.begin(), v2Faces.end(), f1) != v2Faces.end()) {
                // edge from face fv2 was not found in face fv1
                found = f1;
                break;
            }
        }
        if(found != nullptr) {
            // v1 and v2 are used in the same face
            // split the face using manifoldMakeEdge
            Edge* newe = new Edge();
            Face* newf = new Face();
            ManifoldMakeEdge(v1, v2, found, newe, newf);
            common = newe;
        }
    }

    // find edge pairs and their appropriate vertices.
    // edge pairs are pairs of edges(e1, e2), where e1 is from v1->Edges() and v2 is from v2->Edges()
    // such that e1->Other(v1) == ev->Other(v2)
    std::vector<Edge*> v1PairEdges = std::vector<Edge*>();
    std::vector<Edge*> v2PairEdges = std::vector<Edge*>();
    std::vector<Edge*> v2OtherEdges = std::vector<Edge*>();

    for(Edge* v2Edge : v2Edges) {
        bool found = false;
        for(Edge* v1Edge : v1Edges) {
            if(v1Edge->Other(v1) == v2Edge->Other(v2)) {
                v1PairEdges.push_back(v1Edge);
                v2PairEdges.push_back(v2Edge);
                found = true;
                break;
            }
        }
        if(!found) {
            // edge of v2 does not share vertices with any edges of v1
            if(v2Edge != common) {
                v2OtherEdges.push_back(v2Edge);
            }
        }
    }

    // find suspicious face pairs
    // suspicious face pairs are pairs of faces(f1 from v1->faces(), f2 from v2->faces()),
    // such that edges from two edge pairs(v1, v2) are utilized in the face loop
    // if first pair is named (e11, e21) and second pair (e21, e22),
    // f1 contains e11, e12, and f2 contains e21, e22
    // mergeable faces are suspicious face pairs, which use the same edges inside their loops,
    // except the edges currently being merged.
    // if mergeable faces are found, delete faces from face pairs which utilize v2
    if(v1PairEdges.size() > 1) {
        for(int i = 0; i < v1PairEdges.size() - 1; ++i) {
            for(int j = i + 1; j < v1PairEdges.size(); ++j) {
                deleteMergeableFaces(v1PairEdges.at(i), v1PairEdges.at(j), v2PairEdges.at(i), v2PairEdges.at(j));
            }
        }
    }

    if(common != nullptr) {
        // check if common edge has faces
        // TODO: this can likely be optimized
        for(Face* face : common->Faces()) {
            if(face->Loops().size() <= 3) {
                // faces with loops < 3 should not exist, but check anyways
                // if face has 3 loops, this means that it's a triangle
                // therefore, it's edges are already inside pairEdges lists.
                // kill the face.
                KillFace(face);
            } else {
                // face has 4 or more loops, therefore, it will remain
                // find the loop which uses this edge and remove it from face list via fNext, fPrev
                std::vector<Loop*> loops = common->Loops();
                for(Loop* loop : loops) {
                    if(loop->f == face) {
                        // remove loop from face list of loops
                        loop->fNext->fPrev = loop->fPrev;
                        loop->fPrev->fNext = loop->fNext;
                        if(face->l == loop) {
                            face->l = loop->fNext;
                        }
                        // remove loop from edge list of loops
                        if(loop->eNext == loop && loop->ePrev == loop) {
                            // this is the last loop of this edge
                            common->l = nullptr;
                        } else {
                            common->l = loop->eNext;
                            loop->ePrev->eNext = loop->eNext;
                            loop->eNext->ePrev = loop->ePrev;
                        }
                        delete loop;
                        break;
                    }
                }
            }
        }
        // at this point, nothing should reference the common edge, kill it.
        // this will remove it from v1, v2 lists.
        KillEdge(common);
    }

    // for edges without a pair which will not be merged,
    // remove all v2 references from edges and loops and replace them with references to v1
    // add the edge to list of edges around v1
    for(Edge* edge : v2OtherEdges) {
        if(edge->v1 == v2) {
            edge->v1 = v1;
            if(v1->e == nullptr) {
                v1->e = edge;
                edge->v1Next = edge;
                edge->v1Prev = edge;
            } else {
                // v1 already has some edges
                Edge* current = v1->e;
                Edge* prev = current->Prev(v1);
                // v1->e = edge;

                edge->v1Prev = prev;
                edge->v1Next = current;

                // must check wether v1 is v1 or v2 in current and previous edge
                if(current->v1 == v1) {
                    current->v1Prev = edge;
                } else {
                    current->v2Prev = edge;
                }

                if(prev->v1 == v1) {
                    prev->v1Next = edge;
                } else {
                    prev->v2Next = edge;
                }
            }
        } else {
            edge->v2 = v1;
            if(v1->e == nullptr) {
                v1->e = edge;
                edge->v2Next = edge;
                edge->v2Prev = edge;
            } else {
                // v1 already has some edges
                Edge* current = v1->e;
                Edge* prev = current->Prev(v1);
                // v1->e = edge;

                edge->v2Prev = prev;
                edge->v2Next = current;

                // must check wether v1 is v1 or v2 in current and previous edge
                if(current->v1 == v1) {
                    current->v1Prev = edge;
                } else {
                    current->v2Prev = edge;
                }

                if(prev->v1 == v1) {
                    prev->v1Next = edge;
                } else {
                    prev->v2Next = edge;
                }
            }
        }

        for(Loop* loop : edge->Loops()) {
            if(loop->v == v2) {
                loop->v = v1;
            }
        }
    }

    // for edges with pairs
    // replace all references to e2 edges with e1 edges
    // delete e2 edges
    for(std::size_t i = 0; i < v2PairEdges.size(); ++i) {
        Edge* edge = v2PairEdges.at(i);
        Edge* pairEdge = v1PairEdges.at(i);
        for(Loop* loop : edge->Loops()) {
            loop->e = pairEdge;
            if(loop->v == v2) {
                loop->v = v1;
            }
            // add loop to list of loops around e
            if(pairEdge->l == nullptr) {
                pairEdge->l = loop;
                loop->eNext = loop;
                loop->ePrev = loop;
            } else {
                // edge already has some adjecent loops
                Loop* currentLoop = pairEdge->l;
                Loop* prevLoop = currentLoop->ePrev;
                pairEdge->l = loop;

                loop->ePrev = prevLoop;
                loop->eNext = currentLoop;
                currentLoop->ePrev = loop;
                prevLoop->eNext = loop;
            }
        }

        // remove edge from list of edges around pair vert.
        // other edge will always have at least one edge connected
        // otherwise it wouldn't be in this list
        Edge* prev;
        Edge* next;
        Vert* other = edge->Other(v2);
        if(edge->v2 == other) {
            // use v2next, v2prev
            prev = edge->v2Prev;
            next = edge->v2Next;
        } else {
            // use v1next, v1prev
            prev = edge->v1Prev;
            next = edge->v1Next;
        }

        if(next->v1 == other) {
            next->v1Prev = prev;
        } else {
            next->v2Prev = prev;
        }

        if(prev->v1 == other) {
            prev->v1Next = next;
        } else {
            prev->v2Next = next;
        }

        // remove edge from mesh list of edges
        if(edge->mNext == edge && edge->mPrev == edge) {
            edge->m->edges = nullptr;
        } else {
            edge->mPrev->mNext = edge->mNext;
            edge->mNext->mPrev = edge->mPrev;
            if(edge->m->edges == edge) {
                edge->m->edges = edge->mNext;
            }
        }
        delete edge;
    }

    // remove v2 from mesh list of verts
    // not checking if v2 is last, because v1 must remain
    v2->mPrev->mNext = v2->mNext;
    v2->mNext->mPrev = v2->mPrev;
    if(v2->m->verts == v2) {
        v2->m->verts = v2->mNext;
    }
    delete v2;

    return;
}

} // namespace Core
} // namespace Aoba
