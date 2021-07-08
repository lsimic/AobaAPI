#include "AobaAPI/Ops/Modify.hpp"

namespace Aoba {
namespace Ops {

const ExtrudeEdgesResult ExtrudeEdges(Core::Mesh* m, const std::vector<Core::Edge*>& edges) {
    // TODO: check if all elements belong to the mesh

    ExtrudeEdgesResult result = ExtrudeEdgesResult();

    std::vector<Core::Edge*> horizontalEdges = m->edgePool.Allocate(edges.size());
    std::vector<Core::Face*> faces = m->facePool.Allocate(edges.size());
    std::vector<Core::Loop*> loops = m->loopPool.Allocate(edges.size() * 4);

    // not exact cause some verts might be shared, and some not
    // new verts and vertical edges are allocated one by one
    // might incur a slight performance hit
    std::vector<Core::Vert*> verts = std::vector<Core::Vert*>();
    verts.reserve(edges.size()); 
    m->vertPool.Reserve(edges.size());

    std::vector<Core::Edge*> verticalEdges = std::vector<Core::Edge*>();
    verticalEdges.reserve(edges.size());
    m->edgePool.Reserve(edges.size());

    const int32_t EXTRUDED_VERT = 1 << 0;

    int vertIdx = 0;
    for(std::size_t i = 0; i < edges.size(); ++i) {
        std::vector<Core::Vert*> currentVerts = edges.at(i)->Verts();
        // extrude all individual verts into vertical edges ig not already extruded
        for(int j = 0; j < 2; ++j) {
            if(!(currentVerts.at(j)->flagsIntern & EXTRUDED_VERT)) {
                Core::Vert* newv = m->vertPool.Allocate();
                Core::Edge* newe = m->edgePool.Allocate();

                Core::MakeEdgeVert(m, currentVerts.at(j), newe, newv);
                newv->co = currentVerts.at(j)->co;

                // set flags and index on the vert.
                currentVerts.at(j)->flagsIntern = EXTRUDED_VERT;
                currentVerts.at(j)->index = vertIdx;
                vertIdx++;

                // push new edge, vert
                verticalEdges.push_back(newe);
                verts.push_back(newv);
            }
        }

        // create the horizontal edge, accessing the new verts by index stored on existing verts of the current edge.
        Core::Edge* newe = horizontalEdges.at(i);
        Core::MakeEdge(m, verts.at(currentVerts.at(0)->index), verts.at(currentVerts.at(1)->index), newe);

        // find edges and verts which form the face boundary, keeping in mind existing loops
        // try to avoid discontinued normals
        std::vector<Core::Edge*> faceEdges = std::vector<Core::Edge*>();
        faceEdges.reserve(4);
        faceEdges.push_back(edges.at(i));
        Core::Vert* loopVert = currentVerts.at(0);
        if(edges.at(i)->IsBoundary()) {
            loopVert = edges.at(i)->Loops().at(0)->LoopVert();
        }
        Core::Vert* first = edges.at(i)->Other(loopVert);
        faceEdges.push_back(verticalEdges.at(loopVert->index));
        faceEdges.push_back(horizontalEdges.at(i));
        faceEdges.push_back(verticalEdges.at(first->index));
        std::vector<Core::Vert*> faceVerts = {first, loopVert, verts.at(loopVert->index), verts.at(first->index)};
        
        // create the new face
        Core::Face* newf = faces.at(i);
        std::vector<Core::Loop*> faceLoops =
            std::vector<Core::Loop*>(loops.begin() + (4 * i), loops.begin() + (4 * i + 4)); 
        Core::MakeLoop(m, faceEdges, faceVerts, faceLoops);
        Core::MakeFace(m, faceLoops.at(0), newf);
    }

    // clear flags and index from verts
    for(std::size_t i = 0; i < edges.size(); ++i) {
        std::vector<Core::Vert*> currentVerts = edges.at(i)->Verts();
        for(int j = 0; j < 2; ++j) {
            currentVerts.at(j)->index = 0;
            currentVerts.at(j)->flagsIntern = 0;
        }
    }

    result.faces = faces;
    result.horizontalEdges = horizontalEdges;
    result.verticalEdges = verticalEdges;
    result.verts = verts;

    return result;
}

} // namespace Ops
} // namespace Aoba
