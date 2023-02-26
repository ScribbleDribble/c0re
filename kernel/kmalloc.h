#include "vmm.h"

/*
    os dev 
1. do we have enough free virtual memory?
2. if so, use some structure to record memory allocation status (depends on the allocator)
3. if not, then ask the VMM to enlarge available address space (sbrk/mmap/mapkernelheap etc.)
4. VMM in turn calls the PMM to allocate RAM
5. the newly allocated RAM is recorded in the appropriate paging tables by the VMM
6. go to step 2.
*/
 
// 32 block_header_t. contains block size and blocks used (3 bits)
// payload to have requested size in bytes. memset payload base address from 0 to requested_size.
// padding. pad out bits to nearest multiple of 8.
// another 32 block_header_t. contains block size and blocks used (3 bits)
typedef struct block_header_t {
    uint32_t space : 3;
    uint32_t block_size : 29
}block_header_t;


// 8-bit aligned bh 
block_header_t root_bh;


block_header_t create_block_header();

void first_fit();

void* kmalloc(uint32_t size);
void init_kmalloc();