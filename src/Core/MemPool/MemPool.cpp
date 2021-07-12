#include "AobaAPI/Core/MemPool/MemPool.hpp"

#include "AobaAPI/Core/Mesh/Edge.hpp"
#include "AobaAPI/Core/Mesh/Face.hpp"
#include "AobaAPI/Core/Mesh/Loop.hpp"
#include "AobaAPI/Core/Mesh/Vert.hpp"

namespace Aoba {
namespace Core {

template<typename T>
MemPool<T>::MemPool() {
    chunks = std::vector<MemPoolChunk<T>*>();
}

template<typename T>
MemPool<T>::~MemPool() {
    for(MemPoolChunk<T>* chunk : chunks) {
        delete chunk;
    }
}

template<typename T>
T* MemPool<T>::Allocate() {
    // TODO: could perhaps implement a way to mark the first/last full/empty chunk
    // to avoid looping over all chunks
    if(chunks.size()) {
        for(MemPoolChunk<T>* chunk : chunks) {
            if(chunk->CanAllocate()) {
                return chunk->Allocate();
            }
        }
    }
    // no chunks with space available, create a new chunk
    MemPoolChunk<T>* chunk = new MemPoolChunk<T>(32);
    chunks.push_back(chunk);
    return chunk->Allocate();
}

template<typename T>
void MemPool<T>::Free(T* item) {
    if(chunks.size()) {
        for(auto it = chunks.begin(); it != chunks.end(); ++it) {
            MemPoolChunk<T>* chunk = (*it);
            if(chunk->IsInChunk(item)) {
                chunk->Free(item);
            }
            if(chunk->IsEmpty()) {
                chunks.erase(it);
            }
            return;
        }
        throw; // data not found in mempool
    }
    throw; // mempool empty
}

template<typename T>
std::vector<T*> MemPool<T>::Allocate(std::size_t count) {
    std::vector<T*> result = std::vector<T*>();
    result.reserve(count);

    if(chunks.size()) {
        for(auto it = chunks.rbegin(); it != chunks.rend(); ++it) {
            if(!(*it)->IsFull()) {
                // found a chunk with some space available
                if(count < (*it)->Available()) {
                    std::vector<T*> allocated = (*it)->Allocate(count);
                    result.insert(result.end(), allocated.begin(), allocated.end());
                    return result;
                } else {
                    count = count - (*it)->Available();
                    std::vector<T*> allocated = (*it)->Allocate((*it)->Available());
                    result.insert(result.end(), allocated.begin(), allocated.end());
                }
            }
        }
    }
    // filled all available chunks, create a new chunk to fit the rest
    // no chunks with space available, create a new chunk that can fit the remaining elements
    std::size_t size = (count < 32) ? 32 : count; 
    MemPoolChunk<T>* chunk = new MemPoolChunk<T>(size); 
    chunks.push_back(chunk);
    std::vector<T*> allocated = chunk->Allocate(count);
    result.insert(result.end(), allocated.begin(), allocated.end());
    return result;
}

template<typename T>
void MemPool<T>::Free(std::vector<T*> items) {
    for(T* ptr : items) {
        for(MemPoolChunk<T>* chunk : chunks) {
            if(chunk->IsInChunk(ptr)) {
                // found chunk which keeps this ptr
                chunk->Free(ptr);
                break;
            }
            // ptr not found in any chunks, throw an exception
            throw;
        }
    }
}

template<typename T>
void MemPool<T>::Reserve(std::size_t count) { 
    count = (count < 32) ? 32 : count; 
    MemPoolChunk<T>* chunk = new MemPoolChunk<T>(count);
    chunks.push_back(chunk);
}

template class MemPool<Edge>;
template class MemPool<Face>;
template class MemPool<Loop>;
template class MemPool<Vert>;

} // namespace Core
} // namespace Aoba