#include "AobaAPI/Ops/Modify.hpp"

namespace Aoba {
namespace Ops {

const TriangulateFacesResult TriangulateFaces(Core::Mesh* m, const std::vector<Core::Face*>& faces) {
    // TODO: use a more advanced triangulation algorithm, which can handle concave cases and complex polygons
    // this currently generates a simple fan triangulation from a single vertex
    std::vector<Core::Edge*> newEdges = std::vector<Core::Edge*>(); 
    std::vector<Core::Face*> triangularFaces = std::vector<Core::Face*>(); 

    for(Core::Face* face : faces) {
        if(face->Loops().size() > 3) {
            // face not a triangle, mark for splitting
            std::vector<Core::Face*> facesToSplit = {face};

            while(!facesToSplit.empty()) {
                // take the last face in list of splittable faces
                Core::Face* current = facesToSplit.back();
                facesToSplit.pop_back();

                // split face using manifoldMakeEdge
                Core::Vert* v1 = current->Loops().at(0)->LoopVert();
                Core::Vert* v2 = current->Loops().at(2)->LoopVert();
                Core::Edge* newe = m->edgePool.Allocate();
                Core::Face* newf = m->facePool.Allocate();
                Core::ManifoldMakeEdge(m, v1, v2, current, newe, newf);
                newEdges.push_back(newe);

                if(current->Loops().size() > 3) {
                    facesToSplit.push_back(current);
                } else {
                    triangularFaces.push_back(current);
                }
                if(newf->Loops().size() > 3) {
                    facesToSplit.push_back(newf); 
                } else {
                    triangularFaces.push_back(newf);
                }
            }
        } else {
            // face already a triangle
            triangularFaces.push_back(face);
        }
    }

    TriangulateFacesResult result = TriangulateFacesResult();
    result.edges = newEdges;
    result.faces = triangularFaces;

    return result;
}

} // namespace Ops
} // namespace Aoba
