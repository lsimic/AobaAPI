#include "AobaAPI/Core/MemPool/MemPoolChunk.hpp"

#include "AobaAPI/Core/Mesh/Edge.hpp"
#include "AobaAPI/Core/Mesh/Face.hpp"
#include "AobaAPI/Core/Mesh/Loop.hpp"
#include "AobaAPI/Core/Mesh/Vert.hpp"

namespace Aoba {
namespace Core {

template<typename T>
MemPoolChunk<T>::MemPoolChunk(std::size_t size) {
    data = new T[size];
    nextFreeIdx = new std::size_t[size]; 
    for(std::size_t i = 0; i < size; ++i) {
        nextFreeIdx[i] = i + 1;
    }
    firstFreeIdx = 0;
    used = 0;
    capacity = size;
}

template<typename T>
MemPoolChunk<T>::~MemPoolChunk() {
    delete[] data;
    delete[] nextFreeIdx;
}

template<typename T>
bool MemPoolChunk<T>::CanAllocate() {
    return used < capacity;
}

template<typename T>
bool MemPoolChunk<T>::CanAllocate(std::size_t count) {
    return count <= (capacity - used);
}

template<typename T>
T* MemPoolChunk<T>::Allocate() {
    if(!CanAllocate()) {
        throw;
    }
    T* result = &data[firstFreeIdx];
    firstFreeIdx = nextFreeIdx[firstFreeIdx];
    ++used;
    return result;
}

template<typename T>
std::vector<T*> MemPoolChunk<T>::Allocate(std::size_t count) {
    if(!CanAllocate(count)) { 
        throw;
    }

    std::vector<T*> result = std::vector<T*>();
    result.reserve(count);
    while(count > 0) {
        result.push_back(&data[firstFreeIdx]);
        firstFreeIdx = nextFreeIdx[firstFreeIdx];
        ++used;
        --count;
    }
    
    return result;
}

template<typename T>
bool MemPoolChunk<T>::IsInChunk(T* item) {
    return item >= data && item < data + capacity;
}

template<typename T>
bool MemPoolChunk<T>::AreInChunk(std::vector<T*> items) {
    for(T* item : items) {
        if(item < data || item >= data + capacity) {
            return false;
        }
    }
    return true;
}

template<typename T>
void MemPoolChunk<T>::Free(T* item) {
    std::size_t idx = item - data;
    nextFreeIdx[idx] = firstFreeIdx;
    firstFreeIdx = idx;
    *item = T();
    item = nullptr;
    --used;
}

template<typename T>
void MemPoolChunk<T>::Free(std::vector<T*> items) {
    for(T* item : items) {
        std::size_t idx = item - data;
        nextFreeIdx[idx] = firstFreeIdx;
        firstFreeIdx = idx;
        *item = T();
        item = nullptr;
        --used;
    }
}

template<typename T>
bool MemPoolChunk<T>::IsEmpty() {
    return used == 0;
}

template<typename T>
bool MemPoolChunk<T>::IsFull() {
    return used == capacity;
}

template<typename T>
std::size_t MemPoolChunk<T>::Available() {
    return capacity - used;
}

template<typename T>
std::size_t MemPoolChunk<T>::Used() {
    return used;
}

template class MemPoolChunk<Edge>;
template class MemPoolChunk<Face>;
template class MemPoolChunk<Loop>;
template class MemPoolChunk<Vert>;

} // namespace Core
} // namespace Aoba