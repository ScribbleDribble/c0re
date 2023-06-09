#ifndef _VMM_H
#define _VMM_H

#include <stdint.h>
#include "../kernel/string.h"
#include <stdbool.h>
#include "pmm.h"


#define PAGE_SIZE 4096
#define PTE_SIZE_BYTES 4
#define PDE_SIZE_BYTES 4
#define PT_SIZE_BYTES PAGE_SIZE * MAX_PTE_COUNT

#define MAX_PTE_COUNT 1024
#define MAX_PDE_COUNT 1024
#define PROCESS_PTE_COUNT MAX_PDE_COUNT*MAX_PTE_COUNT
// #define PT_BASE_ADDR 0x42000 // 100kb into free memory region of lower memory. 4kb aligned
// #define PD_BASE_ADDR 0x21000

#define PT_BASE_ADDR 0x42000 // 100kb into free memory region of lower memory. 4kb aligned
#define PD_BASE_ADDR 0x21000

#define SET_ADDR(entry) (entry |= (uint32_t) pmm_kalloc())
#define GET_ADDR(entry) (entry & 0xfffff000)
#define SET_PRESENT(entry) (entry |= 1)
#define IS_PRESENT(entry) (entry & 1)

#define KERNEL_BINARY_PD_IDX 0xC0 // 0xC0 is PDE containing 
#define KERNEL_HEAP_PD_IDX KERNEL_BINARY_PD_IDX + 1

#define VMM_PRESENT 1

#define KERNEL_VIRT_BASE 0x30000000 // 1GB mark



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

void vmm_init(uint32_t* boot_page_dir, uint32_t* boot_page_table);

void init_page_directory(void);
void create_page_table(uint16_t pd_index);
uint32_t palloc(uint16_t pd_index, int n_allocs);
int mem_map(uint32_t);

//

#endif