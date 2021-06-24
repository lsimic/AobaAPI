#include "AobaAPI/Core/MemPool/MemPool.hpp"

#include "AobaAPI/Core/Mesh/Edge.hpp"
#include "AobaAPI/Core/Mesh/Face.hpp"
#include "AobaAPI/Core/Mesh/Loop.hpp"
#include "AobaAPI/Core/Mesh/Vert.hpp"

namespace Aoba {
namespace Core {

template<typename T>
MemPool<T>::MemPool() {
    chunks = std::vector<MemPoolChunk<T>>();
}

template<typename T>
bool MemPool<T>::HasChunks() {
    return chunks.size();
}

template<typename T>
T* MemPool<T>::AllocateSingle() {
    // TODO: could perhaps implement a way to mark the first/last full/empty chunk
    // to avoid looping over all chunks
    if(HasChunks()) {
        for(auto it = chunks.rbegin(); it != chunks.rend(); ++it) {
            if(!(*it).IsFull()) {
                // found a chunk with some space and got a pointer.
                return (*it).AllocateSingle();
            }
        }
    }
    // no chunks with space available, create a new chunk
    MemPoolChunk<T> chunk = MemPoolChunk<T>(32);
    chunks.push_back(chunk);
    return chunk.AllocateSingle();
}

template<typename T>
void MemPool<T>::FreeSingle(T* item) {
    if(HasChunks()) {
        for(auto it = chunks.begin(); it != chunks.end(); ++it) {
            if((*it).IsSingleInChunk(item)) {
                (*it).FreeSingle(item);
                if((*it).IsEmpty()) {
                    chunks.erase(it);
                }
                return;
            }
        }
        throw; // data not found in mempool
    }
    throw; // mempool empty
}

template<typename T>
std::vector<T*> MemPool<T>::AllocateMany(std::size_t count) {
    std::vector<T*> result = std::vector<T*>();
    result.reserve(count);

    if(HasChunks()) {
        for(auto it = chunks.rbegin(); it != chunks.rend(); ++it) {
            if(!(*it).IsFull()) {
                // found a chunk with some space available
                if(count < (*it).Available()) {
                    std::vector<T*> allocated = (*it).AllocateMany(count);
                    result.insert(result.end(), allocated.begin(), allocated.end());
                    return result;
                } else {
                    count = count - (*it).Available();
                    std::vector<T*> allocated = (*it).AllocateMany((*it).Available());
                    result.insert(result.end(), allocated.begin(), allocated.end());
                }
            }
        }
    }
    // filled all available chunks, create a new chunk to fit the rest
    // no chunks with space available, create a new chunk that can fit the remaining elements
    MemPoolChunk<T> chunk = MemPoolChunk<T>(count);
    chunks.push_back(chunk);
    std::vector<T*> allocated = chunk.AllocateMany(count);
    result.insert(result.end(), allocated.begin(), allocated.end());
    return result;
}

template<typename T>
std::vector<T*> MemPool<T>::AllocateContinous(std::size_t count) {
    MemPoolChunk<T> chunk = MemPoolChunk<T>(count);
    chunks.push_back(chunk);
    return chunk.AllocateMany(count);
}

template<typename T>
void MemPool<T>::FreeMany(std::vector<T*> items) {
    for(T* ptr : items) {
        for(MemPoolChunk<T> chunk : chunks) {
            if(chunk.IsSingleInChunk(ptr)) {
                // found chunk which keeps this ptr
                chunk.FreeSingle(ptr);
                break;
            }
            // ptr not found in any chunks, throw an exception
            throw;
        }
    }
}

template class MemPool<Edge>;
template class MemPool<Face>;
template class MemPool<Loop>;
template class MemPool<Vert>;

} // namespace Core
} // namespace Aoba