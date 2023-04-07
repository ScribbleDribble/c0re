
extern vmm_init
extern page_directory
extern cr2_value

global _enable_paging
global _get_cr2_value
global _simulate_gpf

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

_simulate_gpf:
    push 0xf001
    pop ds                   ; 0xf001 is no valid segment,
    mov ax, [ds:0x12345678] ; let's see if we get the GPF