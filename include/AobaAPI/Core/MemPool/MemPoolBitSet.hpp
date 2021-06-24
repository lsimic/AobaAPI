#ifndef AOBA_CORE_MEMPOOL_MEMPOOLBITSET_HPP
#define AOBA_CORE_MEMPOOL_MEMPOOLBITSET_HPP

#include <cstdlib>

namespace Aoba {
namespace Core {

class MemPoolBitSet {
    int32_t* data;
    std::size_t dataCount;
    std::size_t capacity;
    std::size_t available;
    std::size_t used;
    std::size_t firstFreeIdx;

    std::size_t NextFree(std::size_t idx);

  public:
    MemPoolBitSet(std::size_t nBits);

    ~MemPoolBitSet();

    // test - get a specific bit
    bool Test(std::size_t idx);

    // set - set bit to true
    void Set(std::size_t idx);

    // reset - set bit to false
    void Reset(std::size_t idx);

    // flip - toggle the value of the bit
    void Flip(std::size_t idx);

    // all - check if all bits are set
    bool All();

    // any - check if any bit is set
    bool Any();

    // none - check if no bits are set
    bool None();

    // index of first free bit
    std::size_t NextFree();

    bool Empty();
    bool Full();
    bool HasSpaceSingle();
    bool HasSpaceMany(std::size_t count);
    std::size_t Capacity();
    std::size_t Available();
    std::size_t Used();
};


} // namespace Core
} // namespace Aoba

#endif
