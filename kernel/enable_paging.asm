
extern vmm_init
extern page_directory
extern cr2_value

global _enable_paging
global _get_cr2_value

_enable_paging:
    mov eax, [page_directory]
    mov cr3, eax

    mov eax, cr0
    or eax, 0x80000001
    mov cr0, eax
    
    ret


_get_cr2_value:
    mov eax, cr2
    mov [cr2_value], eax
    ret
