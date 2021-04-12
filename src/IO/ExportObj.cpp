#include "AobaAPI/IO/ExportObj.hpp"

#include <fstream>

namespace Aoba {
namespace IO {

void ExportObj(std::string path, Core::Mesh* mesh) {
    std::ofstream outFile(path);

    outFile << "# AobaAPI test file \n";
    outFile << "o AobaAPIObject \n";

    std::vector<Core::Vert*> verts = mesh->Verts();
    std::vector<Core::Face*> faces = mesh->Faces();
    std::vector<Core::Edge*> edges = mesh->Edges();

    for(std::size_t i = 0; i < verts.size(); ++i) {
        outFile << "v " << verts.at(i)->co.x << " " << verts.at(i)->co.y << " " << verts.at(i)->co.z << " \n";
        verts.at(i)->index = i + 1;
    }

    outFile << "s off \n";

    for(std::size_t i = 0; i < faces.size(); ++i) {
        outFile << "f ";

        // basic obj export, only do the single/first loop
        std::vector<Core::Loop*> loops = faces.at(i)->LoopLists().at(0)->Loops();

        for(std::size_t j = 0; j < loops.size(); ++j) {
            outFile << loops.at(j)->LoopVert()->index << " ";
        }

        outFile << "\n";
    }

    for(std::size_t i = 0; i < edges.size(); ++i) {
        if(edges.at(i)->IsWire()) {
            outFile << "l ";
            for(std::size_t j = 0; j < 2; ++j) {
                outFile << edges.at(i)->Verts().at(j)->index << " ";
            }

            outFile << "\n";
        }
    }

    outFile.close();
}

} // namespace IO
} // namespace Aoba