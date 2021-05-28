#include "AobaAPI/Ops/Modify.hpp"

namespace Aoba {
namespace Ops {

const SubdivideResult SubdivideSmooth(
    Core::Mesh* m, const std::vector<Core::Edge*>& edges, const std::vector<Core::Face*>& faces) {
    SubdivideResult result = SubdivideResult();
    result.edges = std::vector<Core::Edge*>();
    result.edgeVerts = std::vector<Core::Vert*>();
    result.faceVerts = std::vector<Core::Vert*>();
    result.faces = std::vector<Core::Face*>();

    // flags
    const int32_t EDGE_INPUT = 1 << 0;
    const int32_t EDGE_BOUNDARY = 1 << 1;
    const int32_t EDGE_SPLIT = 1 << 2;
    const int32_t EDGE_NEW = 1 << 3;
    const int32_t FACE_INPUT = 1 << 0;
    const int32_t VERT_INPUT = 1 << 0;
    const int32_t VERT_NEW = 1 << 1;
    const int32_t VERT_CENTER = 1 << 2;

    std::vector<Core::Vert*> inputVerts = std::vector<Core::Vert*>();
    std::vector<Core::Edge*> inputEdges = std::vector<Core::Edge*>();

    std::vector<Math::Vec3> faceCenterCoords = std::vector<Math::Vec3>();
    faceCenterCoords.reserve(faces.size());
    std::vector<Math::Vec3> edgeCoords = std::vector<Math::Vec3>();
    std::vector<Math::Vec3> vertCoords = std::vector<Math::Vec3>();

    // calculate faceCenterCoordinates of each input face, find input faces
    std::size_t faceIdx = 0;
    for(Core::Face* face : faces) {
        face->index = faceIdx;
        ++faceIdx;
        faceCenterCoords.push_back(face->CalcCenterAverage());
        face->flagsIntern = FACE_INPUT;
        for(Core::Vert* v : face->Verts()) {
            if(!(v->flagsIntern & VERT_INPUT)) {
                inputVerts.push_back(v);
                v->flagsIntern += VERT_INPUT;
            }
        }
    }

    // calculate new edge center coordinates, find input edges and verrs, find boundary edges
    std::size_t edgeIdx = 0;
    for(Core::Face* face : faces) {
        for(Core::Edge* edge : face->Edges()) {
            if(!(edge->flagsIntern & EDGE_INPUT)) {
                edge->flagsIntern = EDGE_INPUT;
                edge->index = edgeIdx;
                inputEdges.push_back(edge);
                edgeIdx++;
                std::vector<Core::Face*> inputFaces = edge->Faces([FACE_INPUT](const Core::Face* const f) {
                    return (f->flagsIntern == FACE_INPUT);
                });
                if(inputFaces.size() > 1) {
                    Math::Vec3 res = edge->V1()->co + edge->V2()->co;
                    for(Core::Face* f : inputFaces) {
                        res += faceCenterCoords.at(f->index);
                    }
                    res /= (inputFaces.size() + 2);
                    edgeCoords.push_back(res);
                } else {
                    edge->flagsIntern += EDGE_BOUNDARY;
                    edgeCoords.push_back((edge->V1()->co + edge->V2()->co) / 2);
                }
            }
        }
    }
    for(Core::Edge* edge : edges) {
        if(!(edge->flagsIntern & EDGE_INPUT)) {
            edge->flagsIntern = EDGE_INPUT + EDGE_BOUNDARY;
            edge->index = edgeIdx;
            inputEdges.push_back(edge);
            edgeIdx++;
            // not checking face here, if it were touching a face, would have already visited it.
            edgeCoords.push_back((edge->V1()->co + edge->V2()->co) / 2);

            for(Core::Vert* v : edge->Verts()) {
                if(!(v->flagsIntern & VERT_INPUT)) {
                    inputVerts.push_back(v);
                    v->flagsIntern += VERT_INPUT;
                }
            }
        }
    }

    // calculate new vertex coordinates for each input vertex
    std::size_t vertIdx = 0;
    for(Core::Vert* vert : inputVerts) {
        vert->index = vertIdx;
        ++vertIdx;
        // if vertex is boundary(has at least one boundary edge)
        // calculate point as avg(v->co + v->co + boundary edge coordinates)
        // boundary edge in this case also fits the wire edge case.
        int boundaryCount = 0;
        auto vertEdges = vert->Edges();
        Math::Vec3 boundaryEdgePointsSum = Math::Vec3();
        Math::Vec3 avgEdgeCenters = Math::Vec3();
        for(Core::Edge* edge : vertEdges) {
            if(edge->flagsIntern & EDGE_BOUNDARY) {
                boundaryCount++;
                boundaryEdgePointsSum += edgeCoords.at(edge->index);
            }
            if(edge->flagsIntern & EDGE_INPUT && !boundaryCount) {
                avgEdgeCenters += edge->V1()->co;
                avgEdgeCenters += edge->V2()->co;
            }
        }
        avgEdgeCenters /= 2.0f;

        if(boundaryCount) {
            vertCoords.push_back(((2 * vert->co) + boundaryEdgePointsSum) / (boundaryCount + 2));
        } else {
            Math::Vec3 avgFacePoints = Math::Vec3();
            auto vertFaces = vert->Faces();
            for(Core::Face* face : vertFaces) {
                avgFacePoints += faceCenterCoords.at(face->index);
            }
            avgFacePoints /= vertFaces.size();
            avgEdgeCenters /= vertEdges.size(); 


            vertCoords.push_back(
                (avgFacePoints + 2 * avgEdgeCenters + vert->co * (vertFaces.size() - 3.0f)) / vertFaces.size());
        }
    }

    // subdivide all input edges
    for(Core::Edge* edge : inputEdges) {
        Core::Vert* newv = new Core::Vert();
        Core::Edge* newe = new Core::Edge();
        newv->co = edgeCoords.at(edge->index);
        newv->flagsIntern = VERT_NEW;
        edge->flagsIntern += EDGE_SPLIT;
        newe->flagsIntern = EDGE_NEW;

        auto pp = edge->V1()->Edges();

        Core::EdgeSplit(edge, edge->V1(), newe, newv);
        result.edges.push_back(newe);
        result.edges.push_back(edge);
        result.edgeVerts.push_back(newv);

        pp = edge->V1()->Edges();
    }

    // subdivide faces
    for(Core::Face* face : faces) {
        // find newly created verts which must be connected to face center. this list should be ordered
        std::vector<Core::Vert*> verts = face->Verts([VERT_NEW](const Core::Vert* const v) {
            return (v->flagsIntern & VERT_NEW);
        });

        auto pp = verts.at(0)->Edges();
        auto pp2 = verts.at(1)->Edges();

        // initial face split:
        Core::Edge* newe = new Core::Edge();
        Core::Face* newf = new Core::Face();
        Core::ManifoldMakeEdge(verts.at(0), verts.at(1), face, newe, newf);

        pp = verts.at(0)->Edges();
        pp2 = verts.at(1)->Edges();

        Core::Edge* split = new Core::Edge();
        Core::Vert* centerVert = new Core::Vert();
        Core::EdgeSplit(newe, verts.at(0), split, centerVert);
        centerVert->co = faceCenterCoords.at(face->index);
        centerVert->flagsIntern = VERT_CENTER;

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
    for(Core::Vert* vert : result.faceVerts) {
        vert->flagsIntern = 0;
    }
    for(Core::Vert* vert : inputVerts) {
        vert->co = vertCoords.at(vert->index);
        vert->flagsIntern = 0;
        vert->index = 0;
    }
    for(Core::Edge* edge : inputEdges) {
        edge->flagsIntern = 0;
        edge->index = 0;
    }
    for(Core::Face* face : faces) {
        face->flagsIntern = 0;
        face->index = 0;
    }

    return result;
}

} // namespace Ops
} // namespace Aoba