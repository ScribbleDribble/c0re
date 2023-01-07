#ifndef _PMM_H
#define _PMM_H


#include <stdbool.h>
#include <stdint.h>

#include "../drivers/vga.h"

#define ALIGN 4096
#define FREE 0
#define IN_USE 1
#define N_BLOCKS 3500 // 14MB of free physical memory in blocks. 
#define PHYS_BASE 0x0
#define PHYS_END 0x00E00000

// #define PHYS_BASE 0
// #define PHYS_END 0x00100000

#define FREE 0
#define IN_USE 1

#define MEMORY_FULL 999  

uint32_t* pmm_kalloc(void);
bool pmm_kalloc_addr(uint32_t addr);
void pmm_free(void* ptr);
void pmm_init(void);

#endif 