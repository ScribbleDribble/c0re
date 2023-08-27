

#define FREE 0
#define IN_USE 1


#define ALIGN 4096
#define PAGE_SIZE 4096
#define PHYS_BASE 0x800A00000
#define PHYS_END 0x805E00000 // Set this value based on how much virtual memory is mapped and what we may need. Must be divisible by PAGE_SIZE!!
#define N_BLOCKS ((PHYS_END-PHYS_BASE)/PAGE_SIZE)

#define KERNEL_BINARY_PD_IDX 0xC0 // 0xC0 is PDE containing lower addresable memory and the kernel
#define KERNEL_BINARY_PDE_COUNT 3 
#define KERNEL_HEAP_PD_IDX KERNEL_BINARY_PD_IDX - 3 // Places kernel heap below kernel binary in virtual memory
#define KERNEL_BASE_PD_IDX KERNEL_HEAP_PD_IDX - 8 // assuming the kernel heap goes downwards, we will then have a size of 32MB. This is used for defining the beginning of the kernel region of memory
#define KERNEL_STACK_PD_IDX KERNEL_BINARY_PD_IDX - 1

#define PROC_MM_OFFSET 0x401000 // rename to root_pd_pt_size
#define PROC_PT_COUNT 0xa
#define PROC_MM_PD_IDX KERNEL_BINARY_PD_IDX + 1

#define USER_BASE_PD_IDX 0x0

#define VMM_PRESENT 1

#define VIRTUAL_ADDRESS_OFFSET 0x30000000 
#define PTE_SIZE_BYTES 4
#define PDE_SIZE_BYTES 4

#define MAX_PTE_COUNT 1024
#define MAX_PDE_COUNT 1024
#define PROCESS_PTE_COUNT MAX_PDE_COUNT*MAX_PTE_COUNT

#define PT_SIZE_BYTES PAGE_SIZE * MAX_PTE_COUNT

#define MEMORY_WORD_BYTES 4
#define PROC_PD_PT_SIZE ((PROC_PT_COUNT*4096+MAX_PDE_COUNT*PDE_SIZE_BYTES))
