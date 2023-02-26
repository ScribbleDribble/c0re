
#include "pmm.h"
#include "string.h"



// keeps track of what blocks are free in memory - 4MB in memory (4kb * 1000)
static bool bitmap[N_BLOCKS];
static uint32_t tail = 0;

// returns free block within physical memory to be used as a page
uint32_t* pmm_kalloc() {
    int i;
    for(i = 0; i < N_BLOCKS; i++) {
        if (bitmap[i] == (bool) FREE) {
            bitmap[i] = (bool) IN_USE;
            // point to next free block. should check if its not out of bounds
            tail = i+1;
            return (uint32_t*) (ALIGN*i+PHYS_BASE);
        }    
    }
    return (uint32_t*) MEMORY_FULL;
}

bool pmm_kalloc_addr(uint32_t addr) {
    if (!(addr % ALIGN == 0)) {
        kputs("Panic!! Address supplied to kalloc is not 4kb aligned!");
        // TODO kernel panic function (just a while loop)
        while(1);
    }
    if (!(addr >= PHYS_BASE && addr <= PHYS_END)) {
        kputs("Panic!! Address supplied to kalloc is not within range of kernel usable memory");
        while(1);
    }
    int i = (addr - PHYS_BASE) / ALIGN;
    int ret;

    ret = (bitmap[i] == FREE) ? true : false;
    return ret;
}   

void pmm_free(void* ptr) {
    uint32_t idx = (uint32_t) ptr-PHYS_BASE % ALIGN;
    bitmap[idx] = FREE;
}

void pmm_init() {
    int i;
    for (i = 0; i < N_BLOCKS; i++) {
        bitmap[i] = FREE;
    }
}

uint32_t pmm_get_bitmap_tail() {
    return tail;
}
