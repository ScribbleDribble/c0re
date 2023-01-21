
extern vmm_init
extern page_directory

global _enable_paging

_enable_paging:
    mov eax, [page_directory]
    mov cr3, eax

    mov eax, cr0
    or eax, 0x80000001
    mov cr0, eax
    
    ret
