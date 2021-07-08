#ifndef AOBA_CORE_MEMPOOL_MEMPOOL_HPP
#define AOBA_CORE_MEMPOOL_MEMPOOL_HPP

#include "MemPoolChunk.hpp"

#include <vector>

namespace Aoba {
namespace Core {

// TODO: a "reserve" method could be usefull,
// it would reserve the memory required, without givinf it to specific element

template<typename T>
class MemPool {
    std::vector<MemPoolChunk<T>*> chunks;

  public:
    MemPool();

    T* Allocate();

    std::vector<T*> Allocate(std::size_t count);

    void Free(T*);

    void Free(std::vector<T*> data);

    void Reserve(std::size_t count);
};

} // namespace Core
} // namespace Aoba

#endif
