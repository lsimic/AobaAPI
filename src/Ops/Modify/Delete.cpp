#include "AobaAPI/Ops/Modify.hpp"

#include <algorithm>

namespace Aoba {
namespace Ops {

void Delete(Core::Mesh* m, const std::vector<Core::Vert*>& verts, const std::vector<Core::Edge*>& edges,
    const std::vector<Core::Face*>& faces, DeleteMode mode) {
    // delete flag
    const int32_t DELETE = 1 << 0;

    // TODO: some cases which might seem intuitive are not handled somewhat gracefully
    // think about wanting to delete two adjecent faces and the (manifold) edge that they share
    // while this could be achieved by calling delete on that manifold edge, it is not intuitive
    // rather, it is intuitive to call delete on those two adjecent faces, with some delete mode
    // and expect that the shared edge is deleted, while keeping other(boundary) edges.

    // the same can be said when, for example, deleting wire edges joined at a single vertex
    // an intuitive option would be to delete the shared vertex, while keeping the boundary vertices
    // even though the same effect can be achieved by only deleting the shared vertex.

    // verify that no duplicate faces exist in input
    std::vector<Core::Face*> facesToDelete = std::vector<Core::Face*>();
    facesToDelete.reserve(faces.size());
    for(Core::Face* f : faces) {
        if(f->flagsIntern != DELETE) {
            facesToDelete.push_back(f);
            f->flagsIntern = DELETE;
        }
    }

    std::vector<Core::Edge*> edgesToDelete = std::vector<Core::Edge*>();
    if(mode != DeleteMode::FacesOnly) {
        edgesToDelete.reserve(edges.size());
    }

    std::vector<Core::Vert*> vertsToDelete = std::vector<Core::Vert*>();
    if(mode == DeleteMode::All) {
        vertsToDelete.reserve(verts.size());
    }

    // delete faces
    for(Core::Face* f : facesToDelete) {
        // if using DeleteMode other than FaceOnly, mark edges(and verts) of this face for deletion
        if(mode != DeleteMode::FacesOnly) {
            for(Core::FaceLoopList* loopList : f->LoopLists()) {
                for(Core::Loop* loop : loopList->Loops()) {
                    Core::Edge* edge = loop->LoopEdge();
                    if(edge->flagsIntern != DELETE) {
                        edge->flagsIntern = DELETE;
                        edgesToDelete.push_back(edge);
                    }
                    if(mode == DeleteMode::All) {
                        Core::Vert* vert = loop->LoopVert();
                        if(vert->flagsIntern != DELETE) {
                            vert->flagsIntern = DELETE;
                            vertsToDelete.push_back(vert);
                        }
                    }
                }
            }
        }
        // kill the face, keeping its edges and verts alive (for now)
        KillFace(f);
    }

    if(mode == DeleteMode::FacesOnly) {
        return;
    }

    for(Core::Edge* e : edges) {
        if(e->flagsIntern != DELETE) {
            edgesToDelete.push_back(e);
            e->flagsIntern = DELETE;
        }
    }

    // delete edges
    // edges from deleted faces have also been added to edgesToDelete.
    for(Core::Edge* e : edgesToDelete) {
        // if using DeleteMode All, mark verts of this edge for deletion
        if(mode == DeleteMode::All) {
            for(Core::Vert* v : e->Verts()) {
                if(v->flagsIntern != DELETE) {
                    v->flagsIntern = DELETE;
                    vertsToDelete.push_back(v);
                }
            }
        }

        // kill the edge, keeping its verts alive(for now), but killing all adjecent faces
        KillEdge(e);
    }

    if(mode != DeleteMode::All) {
        return;
    }

    for(Core::Vert* v : verts) {
        if(v->flagsIntern != DELETE) {
            vertsToDelete.push_back(v);
            v->flagsIntern = DELETE;
        }
    }

    // kill all verts marked for deletion(either directly or implicit through edges/faces)
    // this will kill other faces and edges as necessary.
    for(Core::Vert* v : vertsToDelete) {
        KillVert(v);
    }
}

} // namespace Ops
} // namespace Aoba