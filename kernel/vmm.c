
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
    memory_set(page_tables, NULL, sizeof(page_tables));
    pmm_init();

    page_table = boot_page_table;
    page_directory = boot_page_dir;
    page_tables[0] = page_table;
    page_dirs[0] = page_directory;
    create_page_table(KERNEL_HEAP_PD_IDX, 0x3);
    palloc(KERNEL_STACK_PD_IDX, MAX_PTE_COUNT, PAGE_P | PAGE_W);
}

static void invalidate_tlb_entry_flush(uint32_t entry) {
    asm("invlpg (%0)" : : "r"(entry));
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
    if (page_dirs[src_pid] == NULL || dest_pid == 0) {
        return NULL;
    }
    klog(">Starting page table cloning procedure.");
    
    // verify new pd starts at correct offset
    uint32_t* dest_pd = (uint32_t) page_dirs[0] + PROC_MM_OFFSET + PROC_PD_PT_SIZE * dest_pid;
    uint32_t* src_pd = page_dirs[src_pid];
    page_dirs[dest_pid] = dest_pd;

    // this is higher-half/kernel memory and quite sensitive - only kernel should r/w
    uint16_t pte_perms = 0x3;
    // allocate page to cover all of src_pid's page table, as we may not have done so already
    // 1. copy page directory from src to dest
    memory_copy(dest_pd, src_pd, 0x1000);
    // klog("src 0x%x, dest 0x%x", (uint32_t)src_pd,  (uint32_t) dest_pd);

    // // 2. we need to set the addresses of the copied page directory entries to point to the new page table locations
    int i;
    for (i = 0; i < PROC_PT_COUNT; i++) {
        // we subtract away VA_OFFSET (0x30,000,000) because the address part in the pde uses a physical address.
        // klog("i:%i - Modifying 0x%x -> 0x%x", i, dest_pd[i], ((uint32_t)dest_pd + 0x1000) + 0x1000 * i - VIRTUAL_ADDRESS_OFFSET);
        SET_ADDR(dest_pd[i], ((uint32_t)dest_pd + 0x1000) + 0x1000 * i - VIRTUAL_ADDRESS_OFFSET);
    }
    // // 3. loop through page directory and for each page directory entry that is PRESENT, we copy that into the new page table 
    for (i = 0; i < PROC_PT_COUNT; i++) {
        if (IS_PRESENT(src_pd[i])) {
            // klog("Copying src PT with address 0x%x into dest PT with address 0x%x", GET_ADDR(src_pd[i]) + VIRTUAL_ADDRESS_OFFSET, GET_ADDR(dest_pd[i] + VIRTUAL_ADDRESS_OFFSET));
            //we add the VA_OFFSET because we access the page table from this virtual address and cpu will perform translation for us
            memory_copy((void*) GET_ADDR(dest_pd[i]) + VIRTUAL_ADDRESS_OFFSET, GET_ADDR(src_pd[i]) + VIRTUAL_ADDRESS_OFFSET, 0x1000);
        }
    }

    assert_true(is_pdir_equal(src_pid, dest_pid), "clone_page_structures failed to copy, new page table is not identical");
    klog(">Finished page table cloning procedure.");

    return dest_pd;
}

void diverge_physical_mappings(uint32_t pid) {
    uint32_t* pd = page_dirs[pid];
    uint32_t* copy_pt = GET_ADDR(page_directory[PROC_VM_COPY_PD_IDX]) + VIRTUAL_ADDRESS_OFFSET;
    uint32_t* link_va  = (uint32_t) 0x400000*PROC_VM_COPY_PD_IDX; // points to new physical address 
    int i;
    int j;
    for (i = 0; i < PROC_PT_COUNT; i++) {
        if (IS_PRESENT(pd[i])) {
            uint32_t* pt = (uint32_t*)(GET_ADDR(pd[i]) + VIRTUAL_ADDRESS_OFFSET);
            for (j = 0; j < MAX_PTE_COUNT; j++) {
                if (IS_PRESENT(pt[j])) {
                    uint32_t dest_phys_addr = (uint32_t) pmm_kalloc();
                    SET_ADDR(copy_pt[0], dest_phys_addr);
                    invalidate_tlb_entry_flush(link_va); // use invpg command instead
                    memory_copy(link_va, PAGE_SIZE*j + i*0x400000, PAGE_SIZE);
                    SET_ADDR(pt[j], dest_phys_addr);
                }
            }
        }
    }
    klog("Divergence of page frame mappings completed.");
}

bool is_pdir_equal(uint16_t pid1, uint16_t pid2) {

    uint32_t* src_pd = page_dirs[pid1];
    uint32_t* dest_pd = page_dirs[pid2];
    int j = 0;
    int i = 0;
    for (; j < MAX_PDE_COUNT; j++) {
        if (IS_PRESENT(src_pd[j])) {
            uint32_t* new_pte = GET_ADDR(dest_pd[j]) + VIRTUAL_ADDRESS_OFFSET;
            uint32_t* old_pte = GET_ADDR(src_pd[j]) + VIRTUAL_ADDRESS_OFFSET;
            for (i = 0; i < MAX_PTE_COUNT; i++) {
                if (IS_PRESENT(*old_pte) && *old_pte != *new_pte){
                    return false;
                }
                new_pte++;
                old_pte++;
            }
        }
    }
    return true;
}

void user_space_vmm_init() {
    uint16_t user_perms = PAGE_W | PAGE_U | PAGE_P;
    memory_set(page_table, 0, 0x1000);
    palloc(USER_BASE_PD_IDX, MAX_PTE_COUNT, user_perms);
	page_directory[USER_BASE_PD_IDX] |= user_perms;

    int i;
    for (i = KERNEL_BINARY_PD_IDX; i < KERNEL_BINARY_PD_IDX+10; i++) {
        klog("pd[i] = 0x%x", page_directory[i]);
    }
    // map physical addresses used for store process page directories and tables
    // 4MB mapped so should support 4MB/0xb000 procs  
    // process page tables use direct mapping (using offsets of 0x30,000,000) so we know which physical address need to be present. 
    page_directory[PROC_MM_PD_IDX] |= PAGE_PS | PAGE_W | PAGE_P;
    SET_ADDR(page_directory[PROC_MM_PD_IDX], 0x400000);
    page_directory[PROC_VM_COPY_PD_IDX] |= PAGE_W | PAGE_P;
    uint32_t* copy_pt = (uint32_t*) GET_ADDR(page_directory[PROC_VM_COPY_PD_IDX]); // PD for copying data between physical addresses for PT copying  
    memory_set(copy_pt, 0, 0x1000);
    palloc(PROC_VM_COPY_PD_IDX, 1, PAGE_W | PAGE_P);

}

uint32_t* reload_cr3(uint32_t target_pid) {
    page_directory = page_dirs[target_pid];
    if (page_directory == NULL) {
        return NULL;
    }
    page_table = (uint32_t*) (GET_ADDR((uint32_t)page_directory[0]) + VIRTUAL_ADDRESS_OFFSET);
    asm("mov %0, %%eax": :"r"((uint32_t) page_directory - VIRTUAL_ADDRESS_OFFSET));
    asm("mov %eax, %cr3");
    return page_directory;
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
 
