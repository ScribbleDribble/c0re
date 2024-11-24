#ifndef _VMM_H
#define _VMM_H


#include <stdint.h>
#include <stdbool.h>
#include "pmm.h"
#include "memory_defs.h"
#include "../common/panic.h"
#include "string.h"


#define SET_ADDR_KALLOC(entry) (entry |= (uint32_t) pmm_kalloc())
#define CLEAR_ADDR(entry) (entry &= 0x00000fff)
#define SET_ADDR(entry, addr) \
    CLEAR_ADDR(entry); \
    entry |= addr
    
#define GET_ADDR(entry) (entry & 0xfffff000)
#define SET_PRESENT(entry) (entry |= 1)
#define IS_PRESENT(entry) (entry & 1)


#define PAGE_P 1
#define PAGE_W 2
#define PAGE_U 4
#define PAGE_WT 8
#define PAGE_CD 16
#define PAGE_AC 32
#define PAGE_AV 64
#define PAGE_PS 128

#define KERNEL_PAGE_TABLE_INDEX 0

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

typedef struct palloc_result_t {
    int rem_allocs;
    uint32_t base_vaddress;
}palloc_result_t;

void vmm_init(uint32_t* boot_page_dir, uint32_t* boot_page_table);
void init_page_directory(void);
void create_page_table(uint16_t pd_index, uint16_t perms);
palloc_result_t palloc(uint16_t pd_index, int n_allocs, uint16_t pte_perms);
int mem_map(uint32_t);
void handle_page_fault(uint32_t ec);
uint32_t* reload_cr3(uint32_t target_pid);
bool is_pdir_equal(uint16_t pid1, uint16_t pid2);
void user_space_vmm_init(void);

#endif