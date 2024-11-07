//
// Created by os on 8/10/24.
//

#include "../lib/mem.h"
#include "../h/MemoryAllocator.hpp"

void* operator new(uint64 n){
    //return __mem_alloc(n);
    return MemoryAllocator::allocate(n);
}

void *operator new[](uint64 n){
    //return __mem_alloc(n);
    return MemoryAllocator::allocate(n);
}

void operator delete(void *p) noexcept{
    //__mem_free(p);
    MemoryAllocator::deallocate(p);
}

void operator delete[](void *p) noexcept{
    //__mem_free(p);
    MemoryAllocator::deallocate(p);
}