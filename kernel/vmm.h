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
#include "pmm.h"

uint32_t create_pde(
    bool present,
    bool writable,
    bool is_userspace,
    bool write_through_policy,
    bool cache_disable,
    bool accessed,
    bool available,
    bool page_size,
    unsigned int pt_address);

uint32_t create_pte(
    const bool present,
    const bool writable,
    const bool is_userspace,
    const bool write_through_policy,
    const bool cache_disable,
    const bool accessed,
    const bool dirty,
    const bool page_attr_table,
    const bool granularity,
    const bool available,
    const uint32_t page_frame_addr
);

void vmm_init(void);


#endif