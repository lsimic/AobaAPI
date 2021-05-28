#include "AobaAPI/Ops/Modify.hpp"

namespace Aoba {
namespace Ops {

const SubdivideResult SubdivideSimple(
    Core::Mesh* m, const std::vector<Core::Edge*>& edges, const std::vector<Core::Face*>& faces) {
    // flags
    const int32_t EDGE_SPLIT = 1 << 0;
    const int32_t EDGE_NEW = 1 << 1;
    const int32_t VERT_NEW = 1 << 0;

    SubdivideResult result = SubdivideResult();
    result.edges = std::vector<Core::Edge*>();
    result.edgeVerts = std::vector<Core::Vert*>();
    result.faceVerts = std::vector<Core::Vert*>();
    result.faces = std::vector<Core::Face*>();

    // subdivide the edges of all input faces
    for(Core::Face* face : faces) {
        for(Core::Edge* edge : face->Edges()) {
            if(edge->flagsIntern == 0) {
                Core::Vert* newv = new Core::Vert();
                Core::Edge* newe = new Core::Edge();
                newv->co = (edge->V1()->co + edge->V2()->co) / 2;

                Core::EdgeSplit(edge, edge->V1(), newe, newv);
                newv->flagsIntern = VERT_NEW;
                newe->flagsIntern = EDGE_NEW;
                edge->flagsIntern = EDGE_SPLIT;

                result.edges.push_back(newe);
                result.edges.push_back(edge);
                result.edgeVerts.push_back(newv);
            }
        }
    }

    // subdivide other input edges
    for(Core::Edge* edge : edges) {
        if(!(edge->flagsIntern & EDGE_SPLIT)) {
            Core::Vert* newv = new Core::Vert();
            Core::Edge* newe = new Core::Edge();
            newv->co = (edge->V1()->co + edge->V2()->co) / 2;

            Core::EdgeSplit(edge, edge->V1(), newe, newv);
            // no need to set tags here, as this is not used by subdivided faces

            result.edges.push_back(newe);
            result.edges.push_back(edge);
            result.edgeVerts.push_back(newv);
        }
    }

    // subdivide faces
    for(Core::Face* face : faces) {
        // find newly created verts which must be connected to face center. this list should be ordered
        std::vector<Core::Vert*> verts = face->Verts([VERT_NEW](const Core::Vert* const v) {
            return (v->flagsIntern == VERT_NEW);
        });
        // find old verts used for center calculation
        std::vector<Core::Vert*> oldVerts = face->Verts([VERT_NEW](const Core::Vert* const v) {
            return (v->flagsIntern != VERT_NEW);
        });

        // calculate original face center
        Math::Vec3 center = Math::Vec3();
        for(Core::Vert* oldVert : oldVerts) {
            center += oldVert->co;
        }
        center = center / static_cast<float>(oldVerts.size());

        // initial face split:
        Core::Edge* newe = new Core::Edge();
        Core::Face* newf = new Core::Face();
        Core::ManifoldMakeEdge(verts.at(0), verts.at(1), face, newe, newf);

        Core::Edge* split = new Core::Edge();
        Core::Vert* centerVert = new Core::Vert();
        Core::EdgeSplit(newe, verts.at(0), split, centerVert);
        centerVert->co = center;

        result.edges.push_back(newe);
        result.edges.push_back(split);
        result.faceVerts.push_back(centerVert);

        Core::Face* faceToSplit = face;
        if(face->Loops().size() == 4) {
            faceToSplit = newf;
            result.faces.push_back(face);
        } else {
            result.faces.push_back(newf);
        }

        for(std::size_t idx = 2; idx < verts.size(); ++idx) {
            Core::Edge* newSplit = new Core::Edge();
            Core::Face* newFace = new Core::Face();
            result.faces.push_back(newFace);
            result.edges.push_back(newSplit);
            Core::ManifoldMakeEdge(centerVert, verts.at(idx), faceToSplit, newSplit, newFace);
            if(newFace->Loops().size() > 4) {
                faceToSplit = newFace;
            }
        }
    }

    // clear flags
    for(Core::Edge* edge : result.edges) {
        edge->flagsIntern = 0;
    }

    for(Core::Vert* vert : result.edgeVerts) {
        vert->flagsIntern = 0;
    }

    return result;
}

} // namespace Ops
} // namespace Aoba