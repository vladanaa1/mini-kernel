//
// Created by os on 8/26/24.
//

#ifndef OS_VERZIJA_POSLE_MORA_MEMORYALLOCATOR_HPP
#define OS_VERZIJA_POSLE_MORA_MEMORYALLOCATOR_HPP

#include "../lib/hw.h"

class MemoryAllocator{
public:
    struct FreeCell{
        uint64* next;
        uint64 size;
        //FreeCell(FreeCell* next, uint64 size): next(next), size(size){}
    };

    static void* allocate(uint64 size);

    static int deallocate(void* pointer);

private:
    static FreeCell* head;

    // void proc_relocate();

    static uint64* compress(FreeCell* first, FreeCell* second);
};

#endif //OS_VERZIJA_POSLE_MORA_MEMORYALLOCATOR_HPP
