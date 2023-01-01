
extern vmm_init

global _enable_paging

_enable_paging:
    call vmm_init
    mov eax, 0x20004A0
    mov cr3, eax

    mov eax, cr0
    or eax, 0x80000001
    mov cr0, eax
    ret



; PAGE_DIRECTORY_PHYS_ADDRESS:
;     db 0x00