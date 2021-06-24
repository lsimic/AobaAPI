#ifndef AOBA_CORE_MEMPOOL_MEMPOOL_HPP
#define AOBA_CORE_MEMPOOL_MEMPOOL_HPP

#include "MemPoolChunk.hpp"

#include <vector>

namespace Aoba {
namespace Core {

template<typename T>
class MemPool {
    std::vector<MemPoolChunk<T>> chunks;

  public:
    MemPool();

    ~MemPool();

    T* AllocateSingle();

    std::vector<T*> AllocateMany(std::size_t count);

    std::vector<T*> AllocateContinous(std::size_t count);

    void FreeSingle(T*);

    void FreeMany(std::vector<T*> data);

    bool HasChunks();

};

} // namespace Core
} // namespace Aoba

#endif
