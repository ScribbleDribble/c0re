global _puts
extern KPUTS_SYSCALL_ID

; eax stores syscall id
; ebx: arg1, ecx: arg2, edx: arg3
_puts:
    xchg bx, bx
    mov eax, KPUTS_SYSCALL_ID
    mov ebx, [esp + 4]
    sysenter
    ret