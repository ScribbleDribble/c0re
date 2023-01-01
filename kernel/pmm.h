#ifndef _PMM_H
#define _PMM_H


#include <stdbool.h>
#include <stdint.h>

#define BLOCK_SZ 4096
#define FREE 0
#define IN_USE 1
#define N_BLOCKS 3500 // 14MB of free physical memory in blocks. 
#define PHYS_BASE 0x00100000
#define PHYS_END 0x00E00000


#define FREE 0
#define IN_USE 1

static bool bitmap[N_BLOCKS];

uint32_t pmm_kalloc();

void pmm_free(void* ptr);

#endif 