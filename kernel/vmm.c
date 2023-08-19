
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
#include "../common/debug.h"

uint32_t* page_dirs[250];
uint32_t* page_tables[250];


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

void vmm_init(uint32_t* boot_page_dir, uint32_t* boot_page_table) {
    memory_set(page_dirs, NULL, sizeof(page_dirs));
    memory_set(page_tables, NULL, sizeof(page_dirs));
    pmm_init();
    page_table = boot_page_table;
    page_directory = boot_page_dir;
    page_tables[0] = page_table;
    page_dirs[0] = page_directory;
    create_page_table(KERNEL_HEAP_PD_IDX, 0x3);   
}

void create_page_table(uint16_t pd_index, uint16_t perms) { 
  
    // since every page table is continguous, find offset to page table start
    int i = MAX_PTE_COUNT*pd_index;
    // calculate end of page table address and iterate till then
    int end_offset = (pd_index*MAX_PTE_COUNT)+MAX_PTE_COUNT;
    for (; i < end_offset; i++) {
        page_table[i] = create_pte(0,0,0,0,0,0,0,0,0,0, 0);
    }
    page_directory[pd_index] |= perms;
}

// returns the page directory of new process
uint32_t* clone_page_structures(uint16_t src_pid, uint16_t dest_pid) {
    if (page_dirs[src_pid] == NULL) {
        return NULL;
    }
    klog(">Starting page table cloning procedure.");

    // verify new pd starts at 4MB + 4kb from prev pt
    // new pd will be at PTs + pd = 4MB + 4kb = 0x401000
    uint32_t* dest_pd = (uint32_t) page_dirs[0] + 0x401000 * dest_pid;
    uint32_t* src_pd = page_dirs[src_pid];

    page_dirs[dest_pid] = dest_pd;

    // this is higher-half/kernel memory and quite sensitive - only kernel should r/w
    uint16_t pte_perms = 0x3;
    // allocate page to cover all of src_pid's page table, as we may not have done so already
    palloc(KERNEL_BINARY_PD_IDX + src_pid, MAX_PTE_COUNT, pte_perms);
    
    // create/reuse next page table to cover all of 0x401000 required memory for copy 
    create_page_table(KERNEL_BINARY_PD_IDX + src_pid+1, pte_perms);
    palloc(KERNEL_BINARY_PD_IDX + src_pid + 1, MAX_PTE_COUNT, pte_perms);

    // create page table for next PD+PT pair. again this will only cover 0x400,000 of the required 0x401000 copy
    create_page_table(KERNEL_BINARY_PD_IDX + dest_pid, pte_perms);
    palloc(KERNEL_BINARY_PD_IDX + dest_pid, MAX_PTE_COUNT, pte_perms);

    create_page_table(KERNEL_BINARY_PD_IDX + dest_pid+1, pte_perms);
    palloc(KERNEL_BINARY_PD_IDX + dest_pid+1, MAX_PTE_COUNT, pte_perms);

    // another page table for the remaining 0x1000. allocating the full page table is overkill but accounts for any offsets

    // 1. copy page directory from src to dest
    memory_copy(dest_pd, src_pd, 0x1000);
    klog("src 0x%x, dest 0x%x", (uint32_t)src_pd,  (uint32_t) dest_pd);
    // 2. we need to set the addresses of the copied page directory entries to point to the new page table locations
    int i;
    for (i = 0; i < MAX_PDE_COUNT; i++) {
        // klog("i:%i - Modifying 0x%x -> 0x%x", i, GET_ADDR(dest_pd[i]), ((uint32_t)dest_pd + 0x1000) + 0x1000 * i - VIRTUAL_ADDRESS_OFFSET);

        // we subtract away VA_OFFSET (0x30,000,000) because the address part in the pde uses a physical address.
        // klog("i:%i - Modifying 0x%x -> 0x%x", i, dest_pd[i], ((uint32_t)dest_pd + 0x1000) + 0x1000 * i - VIRTUAL_ADDRESS_OFFSET);
        SET_ADDR(dest_pd[i], ((uint32_t)dest_pd + 0x1000) + 0x1000 * i - VIRTUAL_ADDRESS_OFFSET);
    }
    // klog("dest end 0x%x", (uint32_t) dest_pd+0x401000);

    // 3. loop through page directory and for each page directory entry that is PRESENT, we copy that into the new page table 
    for (i = 0; i < MAX_PDE_COUNT; i++) {
        if (IS_PRESENT(src_pd[i])) {
            klog("Copying src PT with address 0x%x into dest PT with address 0x%x", GET_ADDR(src_pd[i]) + VIRTUAL_ADDRESS_OFFSET, GET_ADDR(dest_pd[i] + VIRTUAL_ADDRESS_OFFSET));
            //we add the VA_OFFSET because we access the page table from this virtual address and cpu will perform translation for us
            memory_copy((void*) GET_ADDR(dest_pd[i]) + VIRTUAL_ADDRESS_OFFSET, GET_ADDR(src_pd[i]) + VIRTUAL_ADDRESS_OFFSET, 0x1000);
        }
    }
    // use to verify (ish)
    // uint32_t* new_pte = GET_ADDR(dest_pd[USER_BASE_PD_IDX]) + VIRTUAL_ADDRESS_OFFSET;
    // uint32_t* old_pte = GET_ADDR(src_pd[USER_BASE_PD_IDX]) + VIRTUAL_ADDRESS_OFFSET;
    // for (i = 0; i < 5; i++) {
    //     if (IS_PRESENT(*old_pte)){
    //         klog("Actual pte at 0x%x, contains 0x%x Vs 0x%x, with 0x%x ", old_pte, *old_pte, new_pte, *new_pte);
    //     }
    //     new_pte++;
    //     old_pte++;
    // }
    // 4. for each 
    klog(">Finished page table cloning procedure.");

    return dest_pd;
}

// address space sharding will require these steps
/*
    1. copy the page table/ page directory of the parent process
    2. diverge the physical address mappings in newly copied tables
    3. switch address by reloading cr3 value with the PHYSICAL address of the new page directory (v addr-0x30,000,000) 
*/

void diverge_physical_mappings(uint32_t pid) {
    uint32_t* pd = (uint32_t) page_dirs[0] + 0x401000 * pid;
    klog("dest pd:0x%x", pd);
    int i;
    int j;
    for (i = 0; i < MAX_PDE_COUNT; i++) {
        // mappings for kernel will be shared across all processes, so stop diverge
        if (i >= KERNEL_BASE_PD_IDX) {
            break;
        }
        if (IS_PRESENT(pd[i])) {
            uint32_t* pt = (uint32_t*)(GET_ADDR(pd[i]) + VIRTUAL_ADDRESS_OFFSET);
            for (j = 0; j < MAX_PTE_COUNT; j++) {
                if (IS_PRESENT(pt[j])) {
                    uint32_t old_val = pt[j] ;
                    SET_ADDR(pt[j], (uint32_t) pmm_kalloc());
                    klog("pde%i - old pte: 0x%x ---> new pte: 0x%x",i, old_val, pt[j]);
                }
            }
        }
    }

    klog("Divergence of physical page mappings completed.");

}

void user_space_vmm_init() {
    uint16_t user_perms = PAGE_W | PAGE_U | PAGE_P;
    memory_set(page_table, 0, 0x1000);
    palloc(USER_BASE_PD_IDX, MAX_PTE_COUNT, user_perms);
	page_directory[USER_BASE_PD_IDX] |= user_perms;
}

// returns first vaddress of contingous n page allocation
palloc_result_t palloc(uint16_t pd_index, int n_allocs, uint16_t pte_perms) {
    if (n_allocs <= 0 || n_allocs > MAX_PTE_COUNT) {
        panic("panic! Invalid allocation amount");
    }

    page_directory[pd_index] |= 0x3; // todo remove and test

    int start = MAX_PTE_COUNT*pd_index;
    int end = (pd_index*MAX_PTE_COUNT)+MAX_PTE_COUNT;
    int i = start;
    int first_pte_index = -1;

    while (i < end && n_allocs > 0) {
        if (!IS_PRESENT(page_table[i])) {
            SET_PRESENT(page_table[i]);
            SET_ADDR_KALLOC(page_table[i]);
            page_table[i] |= pte_perms;
            n_allocs -= 1;
            if (first_pte_index == -1) {
                first_pte_index = i - start;
            }
        }
        i++;
    }

    if (first_pte_index == -1 || n_allocs > 0){
        kputs("warn: Failed to alloc page within vmm:palloc!");
        palloc_result_t res = {n_allocs, pd_index * PT_SIZE_BYTES + first_pte_index * PAGE_SIZE};
        return res;
    }
    palloc_result_t res = {0, pd_index * PT_SIZE_BYTES + first_pte_index * PAGE_SIZE};
    return res;
}

// TODO: optimise this monstrocity
int mem_map(uint32_t vaddr) {
    int pd_index = 0;

    for(; pd_index < 1024; pd_index++) {
        if (vaddr > PT_SIZE_BYTES*pd_index && vaddr < PT_SIZE_BYTES*(pd_index+1)) {
            int base_index = GET_ADDR(page_directory[pd_index]);
            SET_PRESENT(page_directory[pd_index]);
            int i;
            for (i = base_index; i < base_index+MAX_PTE_COUNT; i++) {
                if (pd_index * PT_SIZE_BYTES + i * PTE_SIZE_BYTES > vaddr)
                if (!IS_PRESENT(page_table[i-1])) {
                    SET_PRESENT(page_table[i-1]);
                    SET_ADDR_KALLOC(page_table[i-1]);
                    return pd_index;
                }
            }
        }
    }
    return -1;
}



// getting the vaddr from a page table is only a matter of finding page's offset. e.g. vaddr = 0x401000
// to find its page, PT 1 ends at 0x400000 (4MB) so its in the second offset of PT 2 (1000-2000)
// vice versa, if we have PT 2's second page, we can find its vaddr by (PD_INDEX * 0x40000 + PT_INDEX * 0x1000)
 
