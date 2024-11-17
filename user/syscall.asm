global _puts
global _fork 
global _getpid
global _sleep

extern KPUTS_SYSCALL_ID
extern FORK_SYSCALL_ID
extern GETPID_SYSCALL_ID
extern SLEEP_SYSCALL_ID

; eax stores syscall id
; ebx: arg1, ecx: arg2, edx: arg3
; saves ESP3 in esi and EIP


_puts:
    mov eax, KPUTS_SYSCALL_ID
    mov ebx, [esp + 4]
    mov edi, [esp] ; get EIP which is always at the top of the stack
    mov esi, esp
    add esi, RET_POP_SIZE  ; simulate ret instruction for when we call sysret
    sysenter

; fs - func  
_fork:
    mov eax, FORK_SYSCALL_ID
    mov edi, [esp]
    mov esi, esp
    add esi, RET_POP_SIZE 
    sysenter

_getpid:
    mov eax, GETPID_SYSCALL_ID
    mov edi, [esp]
    mov esi, esp
    add esi, RET_POP_SIZE 
    sysenter

_sleep:
    xchg bx, bx
    mov eax, SLEEP_SYSCALL_ID
    mov ebx, [esp + 4]
    mov edi, [esp]
    add esi, RET_POP_SIZE
    sysenter


RET_POP_SIZE equ 4