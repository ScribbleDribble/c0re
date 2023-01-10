
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

// Each entry is 4 bytes (32 bits). Bits 31-12 contains the physical address of the page table. Remember, we dont need to have every page table loaded 
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


/*
    malloc() - increase heap for process
        |
        v
       vmm   - will check if any pages are free to use. if not, call kalloc to use up a new page frame
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

static void vmm_logs(void);

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




void init_page_directory() {
    // 1. only use pmm_kalloc for the FREE REGIONS within memory and not reserved parts
    // so do not call pmm_kalloc for reserved memory regions 
    // 2. reserve memory for the page directory and page tables. 

    // identity map lower addressable memory (<1MB) and source the remaining 3MB from usable memory
    memory_set(page_table, 0, 4*1024);
    memory_set(page_directory, 0, 4*1024);
    
    int i;     
    for (i = 0; i < MAX_PTE_COUNT; i++) {
        if (PAGE_SIZE*i < PHYS_BASE) {
            page_table[i] = create_pte(1,1,0,0,0,0,0,0,0,0, i*PAGE_SIZE);
        } else { 
            page_table[i] = create_pte(0,1,1,0,0,0,0,0,0,0, (uint32_t) pmm_kalloc());
        }
    }
    // use shared page tables to initialise page directory. only first page table will be in memory.
    for (i = 0; i < MAX_PD_COUNT; i++) {
        if (i == 0) {
            page_directory[i] = create_pde(1,1,0,0,0,0,0,0, (uint32_t) page_table);
        } else {
            page_directory[i] = create_pde(0,1,1,0,0,0,0,0, (uint32_t) page_table);
        }
    }
}

// id maps <1MB of memory 
// void identity_map_lower_memory() {
//     if (!pmm_kalloc_addr(0x1000) == true) {
//         kputs("Panic!! Can't identity map kernel.");
//         while(1);
//     }

    

//     int i;
//     for (i = 0; i < MAX_PTE_COUNT; i++) {

// +        if (PAGE_SIZE*i >= 1048576) {
//             page_table[i] = create_pte(1,1,1,0,0,0,0,0,0,0, (uint32_t) pmm_kalloc());
//         }
//     }


//     // point second page table to 1:1 map physical addresses hosting kernel to a virtual address of the same location 
//     //page table has already been created by blank_page_directory, we just need to set its page table. 

// }


void vmm_init() {
    pmm_init();
    page_table = (uint32_t*) PT_BASE_ADDR;
    page_directory = (uint32_t*) PD_BASE_ADDR;
    init_page_directory();
    vmm_logs();
}


uint32_t* get_pd(uint8_t index) {
    return page_directory+index;
}

static void vmm_logs() {
    #ifdef DEBUG
        // should point to 0x42000 and have flags as 0x7. res = 0x00042007
        int_to_hex_str(*page_directory, buf, 32);
        kputs(buf);
        // should be address of first page table i.e 0x00042000
        int_to_hex_str(page_table, buf, 32);
        kputs(buf);

        // should point to first 4kb aligned address in memory that is free to use, provided by kalloc. should be 0x00100000. 
        // should also have flags as 0x7
        // so 0x00100007
        int_to_hex_str(*page_table, buf, 32);
        kputs(buf);
        // next entry in page table should point to the next value provided by kalloc. should be 0x00101007
        int_to_hex_str(*(page_table+1), buf, 32);
        kputs(buf);
        // last entry has a physical address, when subtracted with phys address pointed by first pte, produces 4MB difference (4190208). 
        int_to_str((uint32_t)*(page_table+1023)-*(page_table), buf, 32);
        kputs(buf);

        // last entry of page directory should point to the first page table, so its value should be 0x0004200
        int_to_hex_str(*(page_directory + 1023), buf, 32);
        kputs(buf);
    #endif
    
}



 