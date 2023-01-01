
#include "pmm.h"

// region 1 - 0x7e00 - 0x7fff
// region 2 - current PHYS_BASE - PHYS_END

// how do we get know 


// keeps track of what blocks are free in memory - 4MB in memory (4kb * 1000)
static bool bitmap[N_BLOCKS];

// returns free block within physical memory to be used as a page
uint32_t pmm_kalloc() {
    int i;
    for(i = 0; i < N_BLOCKS; i++) {
        if (!(bitmap[i] & IN_USE)) {
            bitmap[i] = (bool) IN_USE;
            return BLOCK_SZ*i+PHYS_BASE;
        }
    }
}

void pmm_free(void* ptr) {
    int idx = ptr-PHYS_BASE % BLOCK_SZ;
    bitmap[idx] = FREE;
}

void pmm_init() {
    // TODO cleaner memset function which takes in a void* pointer
    int i;
    for (i = 0; i < N_BLOCKS; i++) {
        bitmap[i] = FREE;
    }
}
