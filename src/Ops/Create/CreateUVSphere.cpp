#include "AobaAPI/Ops/Create.hpp"

#include <cmath>

namespace Aoba {
namespace Ops {

const CreateUvSphereResult CreateUVSphere(Core::Mesh* m, float radius, std::size_t rings, std::size_t segments) {
    // todo: check if radius, number of rings and number of segments are valid values
    // must have at least three segments and at least two rings

    float segmentAngleStep = (2 * PI) / segments;
    float ringAngleStep = PI / (rings + 1);

    std::vector<Core::Vert*> verts = m->vertPool.Allocate(rings * segments + 2);
    std::size_t vIdx = 0;
    // create verts except the cap verts
    for(int ring = 0; ring < rings; ring++) {
        float ringAngle = ringAngleStep * (1 + ring) - PI / 2;
        float z = radius * sinf(ringAngle);
        float r = radius * cosf(ringAngle);
        for(int segment = 0; segment < segments; segment++) {
            float segmentAngle = segment * segmentAngleStep;
            float x = r * cosf(segmentAngle);
            float y = r * sinf(segmentAngle);

            Core::Vert* newv = verts.at(vIdx);
            ++vIdx;
            Core::MakeVert(m, newv);
            newv->co = Math::Vec3(x, y, z);
        }
    }

    // create cap verts
    Core::Vert* capBottom = verts.at(vIdx);
    ++vIdx;
    Core::MakeVert(m, capBottom);
    capBottom->co = Math::Vec3(0, 0, -radius);
    Core::Vert* capTop = verts.at(vIdx);
    Core::MakeVert(m, capTop);
    capTop->co = Math::Vec3(0, 0, radius);

    std::vector<Core::Edge*> edges = m->edgePool.Allocate((2 * rings + 1) * segments);
    std::size_t eIdx = 0;

    // create ring edges
    std::vector<Core::Edge*> rEdges = std::vector<Core::Edge*>();
    rEdges.reserve(rings * segments);
    for(std::size_t ring = 0; ring < rings; ++ring) {
        for(int i = 0; i < segments; ++i) {
            std::size_t idx = ring * segments + i;
            Core::Edge* newe = edges.at(eIdx);
            ++eIdx;
            if(i < segments - 1) {
                Core::MakeEdge(m, verts.at(idx), verts.at(idx + 1), newe);

            } else {
                Core::MakeEdge(m, verts.at(idx), verts.at(ring * segments), newe);
            }
            rEdges.push_back(newe);
        }
    }

    // create segment edges
    std::vector<Core::Edge*> sEdges = std::vector<Core::Edge*>();
    sEdges.reserve((rings - 1) * segments);
    for(int ring = 0; ring < rings - 1; ++ring) {
        for(int segment = 0; segment < segments; ++segment) {
            std::size_t idx = ring * segments + segment;
            Core::Edge* newe = edges.at(eIdx);
            ++eIdx;
            Core::MakeEdge(m, verts.at(idx), verts.at(idx + segments), newe);
            sEdges.push_back(newe);
        }
    }

    // create bottom cap edges
    std::vector<Core::Edge*> bottomEdges = std::vector<Core::Edge*>();
    bottomEdges.reserve(segments);
    for(int i = 0; i < segments; ++i) {
        Core::Edge* newe = edges.at(eIdx);
        ++eIdx;
        Core::MakeEdge(m, capBottom, verts.at(i), newe);
        bottomEdges.push_back(newe);
    }
    // create top cap edges
    std::vector<Core::Edge*> topEdges = std::vector<Core::Edge*>();
    topEdges.reserve(segments);
    for(std::size_t i = (rings - 1) * segments; i < rings * segments; ++i) {
        Core::Edge* newe = edges.at(eIdx);
        ++eIdx;
        Core::MakeEdge(m, verts.at(i), capTop, newe);
        topEdges.push_back(newe);
    }

    std::vector<Core::Face*> faces = m->facePool.Allocate((rings + 1) * segments);
    std::vector<Core::Loop*> bottomLoops = m->loopPool.Allocate(segments * 3);
    std::size_t fIdx = 0; 
    std::size_t lfIdx = 0; 
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
        std::vector<Core::Loop*> faceLoops =
            std::vector<Core::Loop*>(bottomLoops.begin() + (3 * lfIdx), bottomLoops.begin() + (3 * (lfIdx + 1)));
        Core::MakeLoop(m, loopEdges, loopVerts, faceLoops);
        ++lfIdx;
        Core::MakeFace(m, faceLoops.at(0), faces.at(fIdx));
        ++fIdx; 
    }

    // create top cap faces
    std::vector<Core::Loop*> topLoops = m->loopPool.Allocate(segments * 3);
    lfIdx = 0; 

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
        std::vector<Core::Loop*> faceLoops =
            std::vector<Core::Loop*>(topLoops.begin() + (3 * lfIdx), topLoops.begin() + (3 * (lfIdx + 1)));
        Core::MakeLoop(m, loopEdges, loopVerts, faceLoops);
        ++lfIdx;
        Core::MakeFace(m, faceLoops.at(0), faces.at(fIdx));
        ++fIdx; 
    }

    // create segment faces
    std::vector<Core::Loop*> loops = m->loopPool.Allocate((rings - 1) * segments * 4);
    lfIdx = 0; 

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

            std::vector<Core::Loop*> faceLoops =
                std::vector<Core::Loop*>(loops.begin() + (4 * lfIdx), loops.begin() + (4 * (lfIdx + 1)));
            Core::MakeLoop(m, loopEdges, loopVerts, faceLoops);
            ++lfIdx;
            Core::MakeFace(m, faceLoops.at(0), faces.at(fIdx));
            ++fIdx; 
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
