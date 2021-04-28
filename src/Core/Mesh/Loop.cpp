

#include "AobaAPI/Core/Mesh/Loop.hpp"

#include "AobaAPI/Core/Mesh/Edge.hpp"
#include "AobaAPI/Core/Mesh/Vert.hpp"

#include <stdexcept>

namespace Aoba {
namespace Core {

Vert* Loop::LoopVert() const {
    return v;
}

Edge* Loop::LoopEdge() const {
    return e;
}

Loop::Loop() {
    index = 0;
    flags = 0;
    flagsIntern = 0;
    v = nullptr;
    e = nullptr;
    f = nullptr;
    eNext = nullptr;
    ePrev = nullptr;
    fNext = nullptr;
    fPrev = nullptr;
    m = nullptr;
}

Face* Loop::LoopFace() const {
    return f;
}

} // namespace Core
} // namespace Aoba