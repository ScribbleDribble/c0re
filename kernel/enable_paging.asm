
extern vmm_init
extern page_directory
extern cr2_value
extern boot_page_dir

global _enable_paging
global _get_cr2_value
global _simulate_gpf
global _invalidate_pde0


; _enable_paging:
;     xchg bx, bx

;     mov eax, [esp + 4] ; gets page directory
;     mov cr3, eax

;     mov eax, cr0
;     or eax, 0x80000001
;     mov cr0, eax
    
;     ret


_get_cr2_value:
    mov eax, cr2
    mov [cr2_value], eax
    ret
