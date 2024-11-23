//
// Created by os on 8/26/24.
//

#include "../h/MemoryAllocator.hpp"
#include "../h/print.hpp"

typedef MemoryAllocator::FreeCell FreeCell;

FreeCell * MemoryAllocator::head = nullptr;

void *MemoryAllocator::allocate(uint64 size) {
    if(size <= 0) return nullptr; // ERROR

    /*
    printString1("size is: ");
    printInteger(size);
    printString1("\n");

    printString1("mem block size is: ");
    printInteger(MEM_BLOCK_SIZE);
    printString1("\n");
*/

    size *= MEM_BLOCK_SIZE;

    if(!head){
        head = (FreeCell*) HEAP_START_ADDR;
        head->size = (uint64)HEAP_END_ADDR - (uint64)HEAP_START_ADDR;
        head->next = nullptr;
    }

    FreeCell* prev = nullptr;
    FreeCell* newCell = nullptr; // ostatak posle alociranja memorije uvrstimo u listu
    FreeCell* cur = head;
    while(cur) {
        if(cur->size >= size) break;
        prev = cur;
        cur = (FreeCell*) cur->next;
    }
    if(!cur) return nullptr; // No available memory

    uint64 newSize = cur->size - size;
    if(newSize == 0) newCell = (FreeCell*) cur->next;
    else{
        newCell = (FreeCell*) ((char*)cur + size);
        newCell->size = newSize;
        newCell->next = cur->next;
    }
    if(!prev) head = newCell;
    else prev->next = (uint64*) newCell;

    uint64 bckp = (uint64) cur; // Save size at cur, return cur + 1
    *((uint64*) cur) = size;
    *((uint64*) cur+1) = bckp;
    return ((uint64*) cur + 1);
}

int MemoryAllocator::deallocate(void *pointer) {
    if (pointer < HEAP_START_ADDR || pointer >= HEAP_END_ADDR) return -1; // ERROR

    uint64 size = *((uint64*) pointer - 1);

    FreeCell* cur = head;

    if(head > pointer){
        FreeCell *newCell = (FreeCell*) ((uint64 *) pointer - 1);
        newCell->size = size;
        newCell->next = (uint64*) head;
        head = newCell;
        if((uint64*)(newCell+newCell->size) == newCell->next) compress(newCell, (FreeCell*) newCell->next);
        return 0; // OK
    }

    // FreeCell* prev;

    while(cur){
        if(pointer > cur && (cur->next > pointer || cur->next == nullptr)){
            break;
        }
        // prev = cur;
        cur = (FreeCell*) cur->next;
    }

    if(cur == nullptr) return -1; // ovo nikad ne bi trebalo da se desi

    FreeCell *newCell = (FreeCell*) ((uint64 *) pointer - 1);
    newCell->size = size;
    if(cur->next != nullptr) {
        newCell->next = cur->next;
        if((uint64*)(newCell+newCell->size) == newCell->next) compress(newCell, (FreeCell*)newCell->next);
    }
    else newCell->next = nullptr;
    cur->next = (uint64*) newCell;
    if(cur + cur->size == newCell) compress(cur, newCell);
    // relocate_proc();
    return 0;
}

uint64 *MemoryAllocator::compress(FreeCell *first, FreeCell *second) {
    first->next = second->next;
    first->size += second->size;
    if(second == head) head = first;
    return (uint64*)first;
}


