#include "AobaAPI/IO/ExportStl.hpp"

#include <fstream>

namespace Aoba {
namespace IO {

void ExportStl(std::string path, Core::Mesh* mesh) {
    std::ofstream outFile(path, std::ios::out | std::ios::binary);

    unsigned int triangleCount = 0;

    // write header, empty (80 bytes)
    for(int i = 0; i < 80; ++i) {
        outFile.put(0);
    }

    // write triangle count, populate later (4 bytes, uint32)
    std::streampos triangleCountPos = outFile.tellp();
    outFile.write(reinterpret_cast<const char*>(&triangleCount), sizeof(unsigned int));

    // using simple triangle fan triangulation
    float zero = 0.0f;
    std::vector<Core::Face*> faces = mesh->Faces();
    for(Core::Face* f : faces) {
        std::vector<Core::Loop*> fLoops = f->Loops();
        std::size_t idx = 1;
        while(idx + 1 < fLoops.size()) {
            // increment triangle count
            triangleCount++;

            // write face normal (12 bytes, fp32)
            outFile.write(reinterpret_cast<const char*>(&zero), sizeof(float));
            outFile.write(reinterpret_cast<const char*>(&zero), sizeof(float));
            outFile.write(reinterpret_cast<const char*>(&zero), sizeof(float));

            // write vertex coordinates (3 x 12 bytesm fp32)
            outFile.write(reinterpret_cast<const char*>(&fLoops.at(0)->LoopVert()->co.x), sizeof(float));
            outFile.write(reinterpret_cast<const char*>(&fLoops.at(0)->LoopVert()->co.y), sizeof(float));
            outFile.write(reinterpret_cast<const char*>(&fLoops.at(0)->LoopVert()->co.z), sizeof(float));

            outFile.write(reinterpret_cast<const char*>(&fLoops.at(idx)->LoopVert()->co.x), sizeof(float));
            outFile.write(reinterpret_cast<const char*>(&fLoops.at(idx)->LoopVert()->co.y), sizeof(float));
            outFile.write(reinterpret_cast<const char*>(&fLoops.at(idx)->LoopVert()->co.z), sizeof(float));

            outFile.write(reinterpret_cast<const char*>(&fLoops.at(idx + 1)->LoopVert()->co.x), sizeof(float));
            outFile.write(reinterpret_cast<const char*>(&fLoops.at(idx + 1)->LoopVert()->co.y), sizeof(float));
            outFile.write(reinterpret_cast<const char*>(&fLoops.at(idx + 1)->LoopVert()->co.z), sizeof(float));

            // write attribute byte (2 bytes, uint16)
            outFile.put(0);
            outFile.put(0);

            // increment loop idx
            ++idx;
        }
    }

    // go back, populate triangle count
    outFile.seekp(triangleCountPos);
    outFile.write(reinterpret_cast<const char*>(&triangleCount), sizeof(unsigned int));

    outFile.close();
}

} // namespace IO
} // namespace Aoba