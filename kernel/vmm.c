
// paging basics //

// each process will hold a page table, and each entry of the table is a page.
// physical memory divided into page frames. A single page maps to a page frame
// to translate a virtual address to a physical address, we use the page  + offset, this calculation is performed by the MMU (memory management unit)
// x86 uses two page tables - one to get the base address of page frame and the other to get the offset
// why? we save memory from not putting all of the addresses in one table. so yes, we use paging on the page table itself lol.

// virtual memory //

// with virtual memory, we can in theory load programs with their combined memory usage exceeding main memory capacity.
// this is due to only loading pages when needed (DEMAND PAGING). We only load pages on demand. Inactive pages are stored on disk.
// (e.g. if a region of memory we need access does not have its page loaded in)

// when we have the maximum amount of page frames that are in memory but need to load a new page in, we perform a SWAP.
// we swap a page from disk with a page in memory. the page being swapped out of main memory is known as the VICTIM FRAME :(

// entries within our page table can describe information about a given page. for instance when the PRESENT BIT is set, it tells the processor this page is in memory.
// this information is to help tell our cpu when things go wrong. e.g. if present bit != 1 when a process accesses memory, the cpu generates a page fault exception
// ... the kernel will service the interrupt by loading a page from memory. 

// paging within x86 //

// to enable paging, we must set the 32nd bit of the cr0 register. Note, its the 32nd bit, meaning that we can only set it on protected mode.
// remember, we still have sementation enabled, meaning that we still generate logical addresses when reading or writing from memory
// , and if we enable paging, the cpu will use still use these logical address. good thing we used the flat memory model where offsets start at 0x00000000

// translating virtual addresses to physical
// a linear address (composition of elements to build physical address) composes of 
// pageDirectoryEntry | pageTableEntry |    Offset
// 31                 22              11    0

// page table tables (or the more popular term) page directories. Each process has its own page directory.

/*  

    Page Directory --
                    |--> Page Table 0 (4 bytes)
                    |--> Page Table 1
                    |--> Page Table 2
                    |    ...
                    |--> (up to 1023) some of which are PRESENT - note this is 2^10... 10 the number of bits for pde.

                                          Page Table N --
                                                        |--> pte 0 (4 bytes)
                                                        |--> pte 1
                                                        |--> pte 2 <- these ptes point to a page in physical memory
                                                        |    ...
                                                        |--> (up to 1023)


*/  
// How does the processor know which page directory a process is associated with? By storing the base physical address for the currently running process on CR3.
// bits 12-31 represent this base physical address for that page directory, so we can add the high 12 bits of our virtual address to get the physical address
// of our page table.

// As you can see, we are already in the process of translating our virtual address to a physical one!


// Page Directory Entry Structure //

// Each entry is 4 bytes (32 bits). Bits 31-12 point to the physical address of the page table. 

// Remember, we dont need to have every page table loaded 
// ... within main memory, so we also have a present bit here again as the first bit of the entry - the usefulness for of the 2 level table structure!
// bit 1, W decides if the page table is writable. bit 2, U, when set is suitable for user space/ring 3.


// When writing data to any structure (page, PDE), we need to consider caching writing policies, so that data remains consistent 
// between the source (main-memory or disk) and the cache: 
// 1. cache write through: at the time of writing to cache, we will also write to main memory
// 2. cache write back: we write to the cache but we will write to main memory later. 'Later' could be when we clear cache.
// when bit 3 is set, we write through to cache, else we write back


// a subset of page directories and page tables are cached within a hardware component within the CPU called the TRANSLATION LOOKASIDE BUFFER (TLB
// you can disable caching of a PDE by setting bit 4.

// bit 5 is the accessed bit. it is set by the processor when it is accessed. can only be unset by the kernel. 

// bit 7 - When set, each page within the table will be 4MB, and 4kb when 0. When set, we no longer have a two level structure, and each page frame
// will infact just be each PDE. Why 4MB? Remember each page is 4kb so 4kb * 1000 = 4MB

// bits 6,8,9,10, are ignored

// Page structure //

// Each entry is the same as the page directory (again because a PDE can be page itself), so it is 4 bytes in size. There are some differences
// bit 7 here can be ignored, as only PDEs should set the size of the pages. 
// bit 6 was ignored by PDEs but can be set here - this is known as the dirty bit. Remember the swapping operation? The dirty bit is useful
// ... for identifying when we should write a victim frame's content to disk. Only write the data to disk if the memory contains new data. 
// e.g. if page frame stores just code, it is unlikely to change (if there is no JIT compilation!).
// so it becomes set when memory != disk

// each page frame is 4kb aligned. This means that every page frame's address should end with 000. This leaves us room to mask out the lower 3 hex
// digits. e.g. pmm returns address 0xaa000 so 0xaa123 is a valid pte.


/*
    malloc() - if block of virtual memory runs out, request more (sbrk, mmap)
        |
        v
       vmm   - will call kalloc to allocate new page. returns virtual address of page. 
        |
        v
       pmm    - checks for free blocks within physical memory. returns base addr of page frame to use.  



*/


/** pt--
        |
        |
        -> phys addr: | 20 bit base address of page | flags |

*/
#include "vmm.h"
#include "string.h"
#include "../drivers/vga.h"

char buf[64];

uint32_t* page_table;
uint32_t* page_directory;

uint32_t create_pde(
    bool present,
    bool writable,
    bool is_userspace,
    bool write_through_policy,
    bool cache_disable,
    bool accessed,
    bool available,
    bool page_size,
    unsigned int pt_address
) 
{
    uint16_t flags = 0x0;

    flags |= available;
    flags <<= 1;
    flags |= available;
    flags <<=1;
    flags |= available;
    flags <<=1;
    flags |= available;
    flags <<=1;
    flags |= page_size;
    flags <<=1;
    flags |= available;
    flags <<=1;
    flags |= accessed;
    flags <<=1;
    flags |= cache_disable;
    flags <<=1;
    flags |= write_through_policy;
    flags <<=1;
    flags |= is_userspace;
    flags <<=1;
    flags |= writable;
    flags <<=1;
    flags |= present;

    unsigned int pde = pt_address;
    pde |= flags;

    return pde;
}


uint32_t
 create_pte(
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
) 
{
    uint16_t flags = 0;
    flags |= available;
    flags <<=1;
    flags |= available;
    flags <<=1;
    flags |= available;
    flags <<=1;
    flags |= granularity;
    flags <<=1;
    flags |= page_attr_table;
    flags <<=1;
    flags |= dirty;
    flags <<=1;
    flags |= accessed;
    flags <<=1;
    flags |= cache_disable;
    flags <<=1;
    flags |= write_through_policy;
    flags <<=1;
    flags |= is_userspace;
    flags <<=1;
    flags |= writable;
    flags <<=1;
    flags |= present;
    
    uint32_t pte = page_frame_addr;

    pte |= flags;
    
    return pte;
    
}

// links page directories to expected address of page tables
// identity maps lower addressable memory (<1MB) within the root page table - also built here
void init_paging_structures() {
    int i;     
    for (i = 0; i < MAX_PTE_COUNT; i++) {
        if (PAGE_SIZE*i < PHYS_BASE) {
            page_table[i] = create_pte(1,1,0,0,0,0,0,0,0,0, i*PAGE_SIZE);
        } else { 
            page_table[i] = create_pte(0,1,1,0,0,0,0,0,0,0, 0);
        }
    }
    for (i = 0; i < MAX_PD_COUNT; i++) {
        if (i == 0) {
            page_directory[i] = create_pde(1,1,0,0,0,0,0,0, (uint32_t) page_table);
        } else {
            page_directory[i] = create_pde(0,1,1,0,0,0,0,0, (uint32_t) PT_BASE_ADDR + MAX_PTE_COUNT*i*4);
        }
    }
}


void vmm_init() {
    pmm_init();
    page_table = (uint32_t*) PT_BASE_ADDR;
    page_directory = (uint32_t*) PD_BASE_ADDR;
    init_paging_structures();
    create_page_table(KERNEL_PD_INDEX);
}

void create_page_table(uint16_t pd_index) {
    // since every page table is continguous, find offset to page table start
    int i = MAX_PTE_COUNT*pd_index;
    // calculate end of page table address and iterate till then
    int end_offset = (pd_index*MAX_PTE_COUNT)+MAX_PTE_COUNT;
    for (; i < end_offset; i++) {
        page_table[i] = create_pte(0,1,0,0,0,0,0,0,0,0, 0);
    }
    // maybe call palloc for first pte
    page_directory[pd_index] |= VMM_PRESENT;
}

void* palloc(uint16_t pd_index, int n_allocs) {

    if (n_allocs <= 0 || n_allocs >= MAX_PTE_COUNT) {
        kputs("err: Invalid allocation amount");
        return (void*) -1;
    }
    int i = MAX_PTE_COUNT*pd_index;
    int end = (pd_index*MAX_PTE_COUNT)+MAX_PTE_COUNT;
    void* first_page = (void*) -1;
    while (i < end && n_allocs > 0) {
        if (!IS_PRESENT(page_table[i])) {
            SET_PRESENT(page_table[i]);
            SET_ADDR(page_table[i]);
            n_allocs -= 1;
            if (first_page == (void*) -1) {
                first_page = (void *) page_table[i];
            }
        }
        i++;
    }

    if (first_page == (void*) -1)
        kputs("err: Something went wrong in palloc!");
    return first_page;
}



 
