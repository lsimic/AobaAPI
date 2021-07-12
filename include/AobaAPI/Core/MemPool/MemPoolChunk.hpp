#ifndef AOBA_CORE_MEMPOOL_MEMPOOLCHUNK_HPP
#define AOBA_CORE_MEMPOOL_MEMPOOLCHUNK_HPP

#include <vector>
#include "../EulerOps.hpp"

namespace Aoba {
namespace Core {

class Mesh;

template<typename T>
class MemPoolChunk {
    friend class Mesh;

    T* data;
    std::size_t capacity;
    std::size_t used;

    std::size_t firstFreeIdx;
    std::size_t* nextFreeIdx;

  public:
    MemPoolChunk(std::size_t size);

    ~MemPoolChunk();

    bool CanAllocate();

    bool CanAllocate(std::size_t count);

    T* Allocate();

    std::vector<T*> Allocate(std::size_t count);

    bool IsInChunk(T* item);

    bool AreInChunk(std::vector<T*> items);

    void Free(T* item);

    void Free(std::vector<T*> items);

    bool IsEmpty();

    bool IsFull();
    
    std::size_t Available();

    std::size_t Used();
};

} // namespace Core
} // namespace Aoba

#endif
