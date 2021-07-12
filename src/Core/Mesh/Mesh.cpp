#include "AobaAPI/Core/Mesh/Mesh.hpp"

#include "AobaAPI/Core/Mesh/Edge.hpp"
#include "AobaAPI/Core/Mesh/Face.hpp"
#include "AobaAPI/Core/Mesh/Loop.hpp"
#include "AobaAPI/Core/Mesh/Vert.hpp"
#include "AobaAPI/Math/Matrix/Matrix3.hpp"

namespace Aoba {
namespace Core {

Mesh::Mesh() {
    vertPool = MemPool<Vert>();
    edgePool = MemPool<Edge>();
    loopPool = MemPool<Loop>();
    facePool = MemPool<Face>();
}

void Mesh::Join(Mesh* other) {
    // join verts
    for(MemPoolChunk<Vert>* chunk : other->vertPool.chunks) {
        vertPool.chunks.push_back(chunk);
        for(std::size_t i = 0; i < chunk->capacity; ++i) {
            Vert v = chunk->data[i];
            if(v.m != nullptr) {
                v.m = this;
            }
        }
    }

    // join edges
    for(MemPoolChunk<Edge>* chunk : other->edgePool.chunks) {
        edgePool.chunks.push_back(chunk);
        for(std::size_t i = 0; i < chunk->capacity; ++i) {
            Edge e = chunk->data[i];
            if(e.m != nullptr) {
                e.m = this;
            }
        }
    }

    // join loops
    for(MemPoolChunk<Loop>* chunk : other->loopPool.chunks) {
        loopPool.chunks.push_back(chunk);
        for(std::size_t i = 0; i < chunk->capacity; ++i) {
            Loop l = chunk->data[i];
            if(l.m != nullptr) {
                l.m = this;
            }
        }
    }

    // join faces
    for(MemPoolChunk<Face>* chunk : other->facePool.chunks) {
        facePool.chunks.push_back(chunk);
        for(std::size_t i = 0; i < chunk->capacity; ++i) {
            Face f = chunk->data[i];
            if(f.m != nullptr) {
                f.m = this;
            }
        }
    }


    // all mempool chunks of the other mesh are now used in this mesh
    // therefore, clear the lists from the other mesh
    other->vertPool.chunks.clear();
    other->edgePool.chunks.clear();
    other->loopPool.chunks.clear();
    other->facePool.chunks.clear();

    // now that the chunks are referenced by this mesh, and no longer referenced by the other mesh
    // the other mesh can be deleted
    delete other;
}

bool Mesh::IsValid() const {
    // validate verts
    for(MemPoolChunk<Vert>* chunk : vertPool.chunks) {
        for(std::size_t i = 0; i < chunk->capacity; ++i) {
            Vert v = chunk->data[i];
            if(v.m != nullptr) {
                // check that mesh pointer is correct
                if(v.m != this) {
                    return false;
                }
                if(v.e != nullptr) {
                    // check that first edge in list points to vert
                    if(v.e->v1 != &v && v.e->v2 != &v) {
                        return false;
                    }
                    // check that first edge in list points to mesh
                    if(v.e->m != this) {
                        return false;
                    }
                }
            }
        }
    }

    // validate edges
    for(MemPoolChunk<Edge>* chunk : edgePool.chunks) {
        for(std::size_t i = 0; i < chunk->capacity; ++i) {
            Edge e = chunk->data[i];

            if(e.m != nullptr) {
                // check for self loops
                if(e.v1 == e.v2) {
                    return false;
                }
                // TODO: check for double edges
                // Can do this in O(n) if using flags, if not, it's worse.

                if(e.l != nullptr) {
                    Loop* currentLoop = e.l;
                    // check that loop points to mesh
                    if(currentLoop->m != this) {
                        return false;
                    }
                    // check ordering around verts, vNext/vPrev
                    do {
                        if(currentLoop->e != &e) {
                            return false;
                        }
                        if(currentLoop->ePrev->eNext != currentLoop || currentLoop->eNext->ePrev != currentLoop) {
                            return false;
                        }
                        currentLoop = currentLoop->eNext;
                    } while(currentLoop != e.l);
                }

                // check that v1, v2 point to mesh
                if(e.v1->m != this || e.v2->m != this) {
                    return false;
                }

                // check that mesh pointer is correct
                if(e.m != this) {
                    return false;
                }
            }

        }
    }

    // validate face and face loops
    for(MemPoolChunk<Face>* chunk : facePool.chunks) {
        for(std::size_t i = 0; i < chunk->capacity; ++i) {
            Face f = chunk->data[i];

            if(f.m != nullptr) {
                // check that mesh pointer is correct
                if(f.m != this) {
                    return false;
                }
                // validate face loops
                Loop* currentLoop = f.l;
                do {
                    // check that loop edge, vert point to proper mesh
                    if(currentLoop->e->m != this) {
                        return false;
                    }
                    if(currentLoop->v->m != this) {
                        return false;
                    }
                    // check that loop fNext, fPrev are correct
                    if(currentLoop->fPrev->fNext != currentLoop || currentLoop->fNext->fPrev != currentLoop) {
                        return false;
                    }
                    // check that loop mesh pointer is correct
                    if(currentLoop->m != this) {
                        return false;
                    }

                    currentLoop = currentLoop->fNext;
                } while(currentLoop->fNext != f.l);
            }

        }
    }
      
    return true;
}

void Mesh::Transform(Math::Mat4 mat) {
    Math::Mat3 transformMatrix = Math::Mat3();
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            transformMatrix(i, j) = mat(i, j);
        }
    }

    for(MemPoolChunk<Vert>* chunk : vertPool.chunks) {
        for(std::size_t i = 0; i < chunk->capacity; ++i) {
            Vert v = chunk->data[i];
            if(v.m != nullptr) {
                v.co = transformMatrix * v.co;
                v.co.x += mat(0, 3);
                v.co.y += mat(1, 3);
                v.co.z += mat(2, 3);
            }
        }
    }
}

const std::vector<Vert*> Mesh::Verts() const {
    std::vector<Vert*> result = std::vector<Vert*>();

    for(MemPoolChunk<Vert>* chunk : vertPool.chunks) {
        for(std::size_t i = 0; i < chunk->capacity; ++i) {
            Vert v = chunk->data[i];
            if(v.m != nullptr) {
                result.push_back(&v);
            }
        }
    }
    return result;
}

const std::vector<Edge*> Mesh::Edges() const {
    std::vector<Edge*> result = std::vector<Edge*>();

    for(MemPoolChunk<Edge>* chunk : edgePool.chunks) {
        for(std::size_t i = 0; i < chunk->capacity; ++i) {
            Edge e = chunk->data[i];
            if(e.m != nullptr) {
                result.push_back(&e);
            }
        }
    }
    return result;
}

const std::vector<Face*> Mesh::Faces() const {
    std::vector<Face*> result = std::vector<Face*>();

    for(MemPoolChunk<Face>* chunk : facePool.chunks) {
        for(std::size_t i = 0; i < chunk->capacity; ++i) {
            Face f = chunk->data[i];
            if(f.m != nullptr) {
                result.push_back(&f);
            }
        }
    }
    return result;
}

const std::vector<Vert*> Mesh::Verts(std::function<bool(const Vert* const)> func) const {
    std::vector<Vert*> result = std::vector<Vert*>();

    for(MemPoolChunk<Vert>* chunk : vertPool.chunks) {
        for(std::size_t i = 0; i < chunk->capacity; ++i) {
            Vert v = chunk->data[i];
            if(v.m != nullptr) {
                if(func(&v)) {
                    result.push_back(&v);
                }
            }
        }
    }
    return result;
}

const std::vector<Edge*> Mesh::Edges(std::function<bool(const Edge* const)> func) const {
    std::vector<Edge*> result = std::vector<Edge*>();

    for(MemPoolChunk<Edge>* chunk : edgePool.chunks) {
        for(std::size_t i = 0; i < chunk->capacity; ++i) {
            Edge e = chunk->data[i];
            if(e.m != nullptr) {
                if(func(&e)) {
                    result.push_back(&e);
                }
            }
        }
    }
    return result;
}

const std::vector<Face*> Mesh::Faces(std::function<bool(const Face* const)> func) const {
    std::vector<Face*> result = std::vector<Face*>();

    for(MemPoolChunk<Face>* chunk : facePool.chunks) {
        for(std::size_t i = 0; i < chunk->capacity; ++i) {
            Face f = chunk->data[i];
            if(f.m != nullptr) {
                if(func(&f)) {
                    result.push_back(&f);
                }
            }
        }
    }
    return result;
}

} // namespace Core
} // namespace Aoba