global _enable_syscall
global KPUTS_SYSCALL_ID

extern kputs

; sets up 3 registers required for syscalls
; IA32_SYSENTER_CS - ring 0 code segment selector 
; IA32_SYSENTER_EIP - EIP for routine
; IA32_SYSENTER_ESP - ring 0 ESP (kernel stack) 
; 
; Inside the GDT, Intel's sysenter requires that:
; ring0 code segment, ring0 data segment, ring3 code segment, ring3 data segment are continguous in that order

_enable_syscall:
    xchg bx, bx 
    pusha
    
    
    mov eax, KERNEL_CODE_SELECTOR    ; low 32-bits of MSR 
    mov edx, 0x0    ; high 32-bits of MSR  
    mov ecx, IA32_SYSENTER_CS ; chosen MSR 

    wrmsr   ; write to msr

    mov eax, _syscall_router
    mov edx, 0x0
    mov ecx, IA32_SYSENTER_EIP
    
    wrmsr

    mov eax, 0x90000 ; TODO pls use kernel stack pointer designated to the process 
    mov edx, 0x0
    mov ecx, IA32_SYSENTER_ESP

    wrmsr

    popa 

    ret



_syscall_router:
    
    cmp eax, KPUTS_SYSCALL_ID
    je _kputs

    xchg bx, bx

    iret 

; args:
; const char* str: ebx
_kputs:
    push ebx
    call kputs
    ret




KERNEL_CODE_SELECTOR equ 0x8
IA32_SYSENTER_CS equ 0x174
IA32_SYSENTER_EIP equ 0x176
IA32_SYSENTER_ESP equ 0x175 

KPUTS_SYSCALL_ID equ 0
