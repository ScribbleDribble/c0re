global _enable_syscall
global KPUTS_SYSCALL_ID
global _set_sysenter_esp

extern KSTACK_BASE
extern kputs

; sets up 3 registers required for syscalls
; IA32_SYSENTER_CS - ring 0 code segment selector 
; IA32_SYSENTER_EIP - EIP for routine
; IA32_SYSENTER_ESP - ring 0 ESP (kernel stack) 
; 
; Inside the GDT, Intel's sysenter requires that:
; ring0 code segment, ring0 data segment, ring3 code segment, ring3 data segment are continguous in that order

_enable_syscall:
    ; xchg bx, bx 
    pusha
    
    
    mov eax, KERNEL_CODE_SELECTOR    ; low 32-bits of MSR 
    mov edx, 0x0    ; high 32-bits of MSR  
    mov ecx, IA32_SYSENTER_CS ; chosen MSR 

    wrmsr   ; write to msr

    mov eax, _syscall_router
    mov edx, 0x0
    mov ecx, IA32_SYSENTER_EIP
    
    wrmsr

    mov eax, [KSTACK_BASE] ; TODO pls use kernel stack pointer for the initial process
    mov edx, 0x0
    mov ecx, IA32_SYSENTER_ESP

    wrmsr

    popa 

    ret

; sets kernel stack for syscall. must be the kernel stack for the process
; ESP0 - eax 
_set_sysenter_esp
    xchg bx, bx 
    pusha 
    mov eax, [esp + 32 + 4] ; go up the stack, past saved registers, to retrieve argument 1
    
    ; TODO bounds checking for esp addresses. Should be between (hopefully configurable) start and end kernel stack addresses
    mov edx, 0x0
    mov ecx, IA32_SYSENTER_ESP
    wrmsr 

    popa
    ret 


_syscall_router:
    ; Save esi register (containing ESP3) and edi (for restoring EIP)
    push esi
    push edi

    cmp eax, KPUTS_SYSCALL_ID
    je _kputs



; args:
; const char* str: ebx
_kputs:
    push ebx
    call kputs
    pop ebx 

    jmp _syscall_return


_syscall_return:
    pop edx ; get edi 
    pop ecx ; get eci
    mov eax, 0x202
    push eax 
    popf
    sysexit



KERNEL_CODE_SELECTOR equ 0x8
IA32_SYSENTER_CS equ 0x174
IA32_SYSENTER_EIP equ 0x176
IA32_SYSENTER_ESP equ 0x175 

KPUTS_SYSCALL_ID equ 0
