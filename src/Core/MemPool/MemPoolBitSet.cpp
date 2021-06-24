#include "AobaAPI/Core/MemPool/MemPoolBitSet.hpp"

namespace Aoba {
namespace Core {

MemPoolBitSet::MemPoolBitSet(std::size_t nBits) {
    dataCount = (nBits / 32) + 1;
    capacity = dataCount * 32;
    available = capacity;
    used = 0;
    firstFreeIdx = 0;
    data = new int32_t[dataCount];
}

MemPoolBitSet::~MemPoolBitSet() {
    delete[] data;
}

bool MemPoolBitSet::Test(std::size_t idx) {
    int32_t dataItem = data[idx / 32];
    if(dataItem & 1 << (idx % 32)) {
        return true;
    } else {
        return false;
    }
}

void MemPoolBitSet::Set(std::size_t idx) {
    int32_t dataItem = data[idx / 32];
    dataItem |= (1 << (idx % 32));
    ++used;
    --available;
    if(idx == firstFreeIdx) {
        firstFreeIdx = NextFree(firstFreeIdx);
    }
}

void MemPoolBitSet::Reset(std::size_t idx) {
    int32_t dataItem = data[idx / 32];
    dataItem &= ~(1 << (idx % 32));
    --used;
    ++available;
    if(idx < firstFreeIdx) {
        firstFreeIdx = idx;
    }
}

void MemPoolBitSet::Flip(std::size_t idx) {
    int32_t dataItem = data[idx / 32];
    dataItem ^= (1 << (idx % 32));
}

bool MemPoolBitSet::All() {
    for(std::size_t i = 0; i < dataCount; ++i) {
        if(data[i] != 0xffffffff) {
            return false;
        }
    }
    return true;
}

bool MemPoolBitSet::Any() {
    for(std::size_t i = 0; i < dataCount; ++i) {
        if(data[i] != 0x00000000) {
            return true;
        }
    }
    return false;
}

bool MemPoolBitSet::None() {
    for(std::size_t i = 0; i < dataCount; ++i) {
        if(data[i] != 0x00000000) {
            return false;
        }
    }
    return true;
}

bool MemPoolBitSet::Empty() {
    return used == 0;
}

bool MemPoolBitSet::Full() {
    return available == 0;
}

bool MemPoolBitSet::HasSpaceSingle() {
    return available > 0;
}

bool MemPoolBitSet::HasSpaceMany(std::size_t count) {
    return available > count;
}

std::size_t MemPoolBitSet::NextFree() {
    return firstFreeIdx;
}

std::size_t MemPoolBitSet::NextFree(std::size_t idx) {
    std::size_t dataItemIdx = idx/32;
    std::size_t dataPos = idx%32;

    while(dataItemIdx < dataCount) {
        if(data[dataItemIdx] == 0xffffffff) {
            ++dataItemIdx;
        }
        else {
            while(dataPos < 32) {
                if(data[dataItemIdx] & (1 << dataPos)) {
                    ++dataPos;
                } 
                else {
                    return dataItemIdx*32 + dataPos;
                }
            }
            dataPos = 0;
        }
    }

    // not found, throw an exception
    throw;
}

std::size_t MemPoolBitSet::Capacity() {
    return capacity;
}

std::size_t MemPoolBitSet::Available() {
    return available;
}

std::size_t MemPoolBitSet::Used() {
    return used;
}

} // namespace Core
} // namespace Aoba