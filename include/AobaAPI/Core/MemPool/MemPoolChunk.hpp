#ifndef AOBA_CORE_MEMPOOL_MEMPOOLCHUNK_HPP
#define AOBA_CORE_MEMPOOL_MEMPOOLCHUNK_HPP

#include "MemPoolBitSet.hpp"
#include <vector>

namespace Aoba {
namespace Core {

template<typename T>
class MemPoolChunk {
    T* data;
    MemPoolBitSet bitset;

  public:
    MemPoolChunk(std::size_t capacity);

    ~MemPoolChunk();

    bool CanAllocateSingle();

    bool CanAllocateMany(std::size_t count);

    T* AllocateSingle();

    std::vector<T*> AllocateMany(std::size_t count);

    bool IsSingleInChunk(T* item);

    bool AreManyInChunk(std::vector<T*> items);

    void FreeSingle(T* item);

    void FreeMany(std::vector<T*> items);

    bool IsEmpty();

    bool IsFull();
    
    std::size_t Available();

    std::size_t Used();
};

} // namespace Core
} // namespace Aoba

#endif
