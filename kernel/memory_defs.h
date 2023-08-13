

#define ALIGN 4096
#define FREE 0
#define IN_USE 1
#define N_BLOCKS 400 // 4MB of free physical memory in blocks. TEMPORARY
#define PHYS_BASE 0x800A00000
#define PHYS_END 0x800E00000

#define KERNEL_BINARY_PD_IDX 0xC0 // 0xC0 is PDE containing
#define KERNEL_BINARY_PDE_COUNT 3 
#define KERNEL_HEAP_PD_IDX KERNEL_BINARY_PD_IDX - 1

#define USER_BASE_PD_IDX 0x0

#define VMM_PRESENT 1


#define KERNEL_VIRT_BASE 0x30000000 
#define PAGE_SIZE 4096
#define PTE_SIZE_BYTES 4
#define PDE_SIZE_BYTES 4
#define PT_SIZE_BYTES PAGE_SIZE * MAX_PTE_COUNT

#define MAX_PTE_COUNT 1024
#define MAX_PDE_COUNT 1024
#define PROCESS_PTE_COUNT MAX_PDE_COUNT*MAX_PTE_COUNT

#define MEMORY_WORD_BYTES 4
