#ifndef AOBA_OPS_MODIFY_HPP
#define AOBA_OPS_MODIFY_HPP

#include "AobaAPI/Core.hpp"
#include "AobaAPI/Math.hpp"

#include <vector>

namespace Aoba {
namespace Ops {

enum class DeleteMode { All, FacesOnly, FacesAndEdges };

void Delete(Core::Mesh* m, const std::vector<Core::Vert*>& verts, const std::vector<Core::Edge*>& edges,
    const std::vector<Core::Face*>& faces, DeleteMode mode);

class DuplicateResult {
  public:
    std::vector<Core::Vert*> verts;
    std::vector<Core::Face*> faces;
    std::vector<Core::Edge*> edges;
};

const DuplicateResult Duplicate(Core::Mesh* m, const std::vector<Core::Vert*>& verts,
    const std::vector<Core::Edge*>& edges, const std::vector<Core::Face*>& faces);

class ExtrudeEdgesResult {
  public:
    std::vector<Core::Vert*> verts;
    std::vector<Core::Edge*> horizontalEdges;
    std::vector<Core::Edge*> verticalEdges;
    std::vector<Core::Face*> faces;
};

const ExtrudeEdgesResult ExtrudeEdges(Core::Mesh* m, const std::vector<Core::Edge*>& edges);

class ExtrudeFacesResult {
  public:
    std::vector<Core::Vert*> verts;
    std::vector<Core::Face*> horizontalFaces;
    std::vector<Core::Edge*> horizontalEdges;
    std::vector<Core::Face*> verticalFaces;
    std::vector<Core::Edge*> verticalEdges;
};

const ExtrudeFacesResult ExtrudeFaces(Core::Mesh* m, const std::vector<Core::Face*>& faces, bool keepOrig);

class ExtrudeFaceRegionResult {
    // TODO: perhaps some things could be kept "in place" here
    std::vector<Core::Vert*> verts;
    std::vector<Core::Face*> horizontalFaces;
    std::vector<Core::Edge*> horizontalEdges;
    std::vector<Core::Face*> verticalFaces;
    std::vector<Core::Edge*> verticalEdges;
};

const ExtrudeFaceRegionResult ExtrudeFaceRegion(Core::Mesh* m, const std::vector<Core::Face*>& faces);

class ExtrudeVertsResult {
  public:
    std::vector<Core::Vert*> verts;
    std::vector<Core::Edge*> edges;
};

const ExtrudeVertsResult ExtrudeVerts(Core::Mesh* m, const std::vector<Core::Vert*>& verts);

Core::Face* MakeFace(Core::Mesh* m, const std::vector<Core::Edge*>& edges);

class InsetFacesResult {
  public:
    std::vector<Core::Vert*> verts;
    std::vector<Core::Face*> centerFaces;
    std::vector<Core::Edge*> centerEdges;
    std::vector<Core::Face*> boundaryFaces;
    std::vector<Core::Edge*> boundaryEdges;
};

const InsetFacesResult InsetFaces(Core::Mesh* m, const std::vector<Core::Face*>& faces, float distance);

class InsetFaceRegionResult {
  public:
    std::vector<Core::Vert*> verts;
    std::vector<Core::Face*> centerFaces;
    std::vector<Core::Edge*> centerEdges;
    std::vector<Core::Face*> boundaryFaces;
    std::vector<Core::Edge*> boundaryEdges;
    std::vector<Core::Edge*> interiorEdges;
};

const InsetFaceRegionResult InsetFaceRegion(Core::Mesh* m, const std::vector<Core::Face*>& faces, float distance);

class MirrorResult {
  public:
    std::vector<Core::Vert*> verts;
    std::vector<Core::Face*> faces;
    std::vector<Core::Edge*> edges;
    std::vector<Core::Vert*> mergedVerts;
    std::vector<Core::Face*> mergedFaces;
    std::vector<Core::Edge*> mergedEdges;
};

const MirrorResult Mirror(Core::Mesh* m, const std::vector<Core::Vert*>& verts, const std::vector<Core::Edge*>& edges,
    const std::vector<Core::Face*>& faces, const Math::Vec3& axis, const Math::Vec3& center, float mergeDist);

void RecalculateFaceNormals(Core::Mesh* m, const std::vector<Core::Face*>& faces);

class RegionExtendResult {
  public:
    std::vector<Core::Vert*> verts;
    std::vector<Core::Face*> faces;
    std::vector<Core::Edge*> edges;
};

const RegionExtendResult RegionExtend(Core::Mesh* m, const std::vector<Core::Vert*>& verts,
    const std::vector<Core::Edge*>& edges, const std::vector<Core::Face*>& faces, bool faceStep);

class RegionReduceResult {
  public:
    std::vector<Core::Vert*> verts;
    std::vector<Core::Face*> faces;
    std::vector<Core::Edge*> edges;
};

const RegionReduceResult RegionReduce(Core::Mesh* m, const std::vector<Core::Vert*>& verts,
    const std::vector<Core::Edge*>& edges, const std::vector<Core::Face*>& faces, bool faceStep, bool reduceToZero);

void ReverseFaceOrder(Core::Mesh* m, const std::vector<Core::Face*>& faces);

class SplitEdgesResult {
  public:
    std::vector<Core::Vert*> verts;
    std::vector<Core::Edge*> edges;
};

const SplitEdgesResult SplitEdges(
    Core::Mesh* m, const std::vector<Core::Edge*>& edges, unsigned cuts, const std::vector<float>& ratios);

class TriangulateFacesResult {
  public:
    std::vector<Core::Edge*> edges;
    std::vector<Core::Face*> faces;
};

const TriangulateFacesResult TriangulateFaces(Core::Mesh* m, const std::vector<Core::Face*>& faces);

} // namespace Ops
} // namespace Aoba

#endif