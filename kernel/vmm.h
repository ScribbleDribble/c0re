#ifndef _VMM_H
#define _VMM_H


#define PAGE_DIRECTORY_ADDR 0xa0000000
#define PAGE_SIZE 4096
#define PTE_SIZE 4
#define PDE_SIZE 4

#define MAX_PTE_COUNT 1024
#define MAX_PD_COUNT 1024
// #define PT_BASE_ADDR 0x42000 // 100kb into free memory region of lower memory. 4kb aligned
// #define PD_BASE_ADDR 0x21000

#define PT_BASE_ADDR 0x42000 // 100kb into free memory region of lower memory. 4kb aligned
#define PD_BASE_ADDR 0x21000

#include <stdint.h>
#include "../kernel/string.h"
#include <stdbool.h>

uint32_t create_pde(
    _Bool present,
    _Bool writable,
    _Bool is_userspace,
    _Bool write_through_policy,
    _Bool cache_disable,
    _Bool accessed,
    _Bool available,
    _Bool page_size,
    unsigned int pt_address);

uint32_t create_pte(
    const _Bool present,
    const _Bool writable,
    const _Bool is_userspace,
    const _Bool write_through_policy,
    const _Bool cache_disable,
    const _Bool accessed,
    const _Bool dirty,
    const _Bool page_attr_table,
    const _Bool granularity,
    const _Bool available,
    const uint32_t page_frame_addr
);

void vmm_init();



#endif