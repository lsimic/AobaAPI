#include "AobaAPI/Core/MemPool/MemPoolChunk.hpp"

#include "AobaAPI/Core/Mesh/Edge.hpp"
#include "AobaAPI/Core/Mesh/Face.hpp"
#include "AobaAPI/Core/Mesh/Loop.hpp"
#include "AobaAPI/Core/Mesh/Vert.hpp"

namespace Aoba {
namespace Core {

template<typename T>
MemPoolChunk<T>::MemPoolChunk(std::size_t capacity) : bitset(capacity) {
    data = new T[(capacity / 32 + 1) * 32];
}

template<typename T>
MemPoolChunk<T>::~MemPoolChunk() {
    delete[] data;
}

template<typename T>
bool MemPoolChunk<T>::CanAllocateSingle() {
    return bitset.HasSpaceSingle() > 0;
}

template<typename T>
bool MemPoolChunk<T>::CanAllocateMany(std::size_t count) {
    return bitset.HasSpaceMany(count);
}

template<typename T>
T* MemPoolChunk<T>::AllocateSingle() {
    std::size_t idx = bitset.NextFree();
    bitset.Set(idx);
    data[idx] = T();
    return &data[idx];
}

template<typename T>
std::vector<T*> MemPoolChunk<T>::AllocateMany(std::size_t count) {
    // first check that all items can be allocated
    if(!bitset.HasSpaceMany(count)) {
        throw;
    }

    std::vector<T*> result = std::vector<T*>();
    while(count > 0) {
        std::size_t idx = bitset.NextFree();
        bitset.Set(idx);
        result.push_back(data + idx);
        data[idx] = T();
        --count;
    }
    
    return result;
}

template<typename T>
bool MemPoolChunk<T>::IsSingleInChunk(T* item) {
    return item >= data && item <= data + bitset.Capacity();
}

template<typename T>
bool MemPoolChunk<T>::AreManyInChunk(std::vector<T*> items) {
    for(T* item : items) {
        if(item <= data || item >= data + bitset.Capacity()) {
            return false;
        }
    }
    return true;
}

template<typename T>
void MemPoolChunk<T>::FreeSingle(T* item) {
    bitset.Reset(item - data);
    item = nullptr;
}

template<typename T>
void MemPoolChunk<T>::FreeMany(std::vector<T*> items) {
    for(T* item : items) {
        bitset.Reset(item - data);
        item = nullptr;
    }
}

template<typename T>
bool MemPoolChunk<T>::IsEmpty() {
    return bitset.Empty();
}

template<typename T>
bool MemPoolChunk<T>::IsFull() {
    return bitset.Full();
}

template<typename T>
std::size_t MemPoolChunk<T>::Available() {
    return bitset.Available();
}

template<typename T>
std::size_t MemPoolChunk<T>::Used() {
    return bitset.Used();
}

template class MemPoolChunk<Edge>;
template class MemPoolChunk<Face>;
template class MemPoolChunk<Loop>;
template class MemPoolChunk<Vert>;

} // namespace Core
} // namespace Aoba