#include "AobaAPI/Ops/Create.hpp"

#include <cmath>

namespace Aoba {
namespace Ops {

const CreateUvSphereResult CreateUVSphere(Core::Mesh* m, float radius, std::size_t rings, std::size_t segments) {
    // todo: check if radius, number of rings and number of segments are valid values
    // must have at least three segments and at least two rings

    float segmentAngleStep = (2 * PI) / segments;
    float ringAngleStep = PI / (rings + 1);

    std::vector<Core::Vert*> verts = std::vector<Core::Vert*>();
    verts.reserve(rings * segments + 2);

    // create verts except the cap verts
    for(int ring = 0; ring < rings; ring++) {
        float ringAngle = ringAngleStep * (1 + ring) - PI / 2;
        float z = radius * sinf(ringAngle);
        float r = radius * cosf(ringAngle);
        for(int segment = 0; segment < segments; segment++) {
            float segmentAngle = segment * segmentAngleStep;
            float x = r * cosf(segmentAngle);
            float y = r * sinf(segmentAngle);

            Core::Vert* newv = new Core::Vert();
            Core::MakeVert(m, newv);
            newv->co = Math::Vec3(x, y, z);
            verts.push_back(newv);
        }
    }

    // create cap verts
    Core::Vert* capBottom = new Core::Vert();
    Core::MakeVert(m, capBottom);
    capBottom->co = Math::Vec3(0, 0, -radius);
    Core::Vert* capTop = new Core::Vert();
    Core::MakeVert(m, capTop);
    capTop->co = Math::Vec3(0, 0, radius);
    verts.push_back(capBottom);
    verts.push_back(capTop);

    std::vector<Core::Edge*> edges = std::vector<Core::Edge*>();
    std::size_t edgeCount = (2 * rings + 1) * segments;
    edges.reserve(edgeCount);

    // create ring edges
    std::vector<Core::Edge*> rEdges = std::vector<Core::Edge*>();
    rEdges.reserve(rings * segments);
    for(std::size_t ring = 0; ring < rings; ++ring) {
        for(int i = 0; i < segments; ++i) {
            std::size_t idx = ring * segments + i;
            Core::Edge* newe = new Core::Edge();
            if(i < segments - 1) {
                Core::MakeEdge(verts.at(idx), verts.at(idx + 1), newe);

            } else {
                Core::MakeEdge(verts.at(idx), verts.at(ring * segments), newe);
            }
            rEdges.push_back(newe);
            edges.push_back(newe);
        }
    }

    // create segment edges
    std::vector<Core::Edge*> sEdges = std::vector<Core::Edge*>();
    sEdges.reserve((rings - 1) * segments);
    for(int ring = 0; ring < rings - 1; ++ring) {
        for(int segment = 0; segment < segments; ++segment) {
            std::size_t idx = ring * segments + segment;
            Core::Edge* newe = new Core::Edge();
            Core::MakeEdge(verts.at(idx), verts.at(idx + segments), newe);
            sEdges.push_back(newe);
            edges.push_back(newe);
        }
    }

    // create bottom cap edges
    std::vector<Core::Edge*> bottomEdges = std::vector<Core::Edge*>();
    bottomEdges.reserve(segments);
    for(int i = 0; i < segments; ++i) {
        Core::Edge* newe = new Core::Edge();
        Core::MakeEdge(capBottom, verts.at(i), newe);
        edges.push_back(newe);
        bottomEdges.push_back(newe);
    }
    // create top cap edges
    std::vector<Core::Edge*> topEdges = std::vector<Core::Edge*>();
    topEdges.reserve(segments);
    for(std::size_t i = (rings - 1) * segments; i < rings * segments; ++i) {
        Core::Edge* newe = new Core::Edge();
        Core::MakeEdge(verts.at(i), capTop, newe);
        edges.push_back(newe);
        topEdges.push_back(newe);
    }

    std::vector<Core::Face*> faces = std::vector<Core::Face*>();
    faces.reserve((rings + 1) * segments);

    // create bottom cap faces
    for(std::size_t i = 0; i < segments; ++i) {
        std::vector<Core::Vert*> loopVerts;
        std::vector<Core::Edge*> loopEdges;
        if(i != segments - 1) {
            loopVerts = {verts.at(i), capBottom, verts.at(i + 1)};
            loopEdges = {bottomEdges.at(i), bottomEdges.at(i + 1), rEdges.at(i)};
        } else {
            loopVerts = {verts.at(i), capBottom, verts.at(0)};
            loopEdges = {bottomEdges.at(i), bottomEdges.at(0), rEdges.at(i)};
        }
        Core::Loop* newl = new Core::Loop();
        Core::MakeLoop(loopEdges, loopVerts, newl);
        Core::Face* newf = new Core::Face();
        Core::MakeFace(newl, newf);
        faces.push_back(newf);
    }

    // create top cap faces
    for(std::size_t i = 0; i < segments; ++i) {
        std::size_t idx = (rings - 1) * segments + i;
        std::vector<Core::Vert*> loopVerts;
        std::vector<Core::Edge*> loopEdges;
        if(i != segments - 1) {
            loopVerts = {capTop, verts.at(idx), verts.at(idx + 1)};
            loopEdges = {topEdges.at(i), rEdges.at(idx), topEdges.at(i + 1)};
        } else {
            loopVerts = {capTop, verts.at(idx), verts.at((rings - 1) * segments)};
            loopEdges = {topEdges.at(i), rEdges.at(idx), topEdges.at(0)};
        }
        Core::Loop* newl = new Core::Loop();
        Core::MakeLoop(loopEdges, loopVerts, newl);
        Core::Face* newf = new Core::Face();
        Core::MakeFace(newl, newf);
        faces.push_back(newf);
    }

    // create segment faces
    for(std::size_t ring = 0; ring < rings - 1; ++ring) {
        for(std::size_t segment = 0; segment < segments; ++segment) {
            std::size_t idx = ring * segments + segment;
            Core::Vert* v0 = verts.at(idx);
            Core::Vert* v1;
            Core::Vert* v2;
            Core::Vert* v3 = verts.at(idx + segments);

            Core::Edge* e0 = rEdges.at(idx);
            Core::Edge* e1;
            Core::Edge* e2 = rEdges.at(idx + segments);
            Core::Edge* e3 = sEdges.at(idx);

            if(segment == segments - 1) {
                v1 = verts.at(ring * segments);
                v2 = verts.at((ring + 1) * segments);
                e1 = sEdges.at(ring * segments);
            } else {
                v1 = verts.at(idx + 1);
                v2 = verts.at(idx + segments + 1);
                e1 = sEdges.at(idx + 1);
            }

            std::vector<Core::Edge*> loopEdges = {e0, e1, e2, e3};
            std::vector<Core::Vert*> loopVerts = {v0, v1, v2, v3};

            Core::Loop* newl = new Core::Loop();
            Core::MakeLoop(loopEdges, loopVerts, newl);
            Core::Face* newf = new Core::Face();
            Core::MakeFace(newl, newf);

            faces.push_back(newf);
        }
    }

    CreateUvSphereResult result = CreateUvSphereResult();
    result.verts = verts;
    result.edges = edges;
    result.Faces = faces;

    return result;
}

} // namespace Ops
} // namespace Aoba
