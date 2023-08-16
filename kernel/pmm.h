#ifndef _PMM_H
#define _PMM_H


#include <stdbool.h>
#include <stdint.h>

#include "../drivers/vga.h"
#include "memory_defs.h"

#define FREE 0
#define IN_USE 1

#define MEMORY_FULL 999  

uint32_t* pmm_kalloc(void);
bool pmm_kalloc_addr(uint32_t addr);
void pmm_free(void* ptr);
void pmm_init(void);
uint32_t get_total_usable_space_bytes();


#endif 