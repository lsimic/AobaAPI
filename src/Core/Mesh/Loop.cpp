

#include "AobaAPI/Core/Mesh/Loop.hpp"

#include "AobaAPI/Core/Mesh/Edge.hpp"
#include "AobaAPI/Core/Mesh/Vert.hpp"

#include <stdexcept>

namespace Aoba {
namespace Core {

Vert* Loop::Vert() const {
    return v;
}

Edge* Loop::Edge() const {
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

} // namespace Core
} // namespace Aoba