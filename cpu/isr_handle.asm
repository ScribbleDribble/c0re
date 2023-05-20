extern interrupt_handler
;  cpu looks inside idt to find which isr will handle cpu generated interrupt. OS then takes over code as seen here. 
; isrs will prepare our stack for our interrupt handlers. Given a cpu exception, the cpu might push a value onto the stack
; to keep a uniform stack, all isrs will push an error code. 

global _isr0
global _isr1
global _isr2
global _isr3
global _isr4
global _isr5
global _isr6
global _isr7
global _isr8
global _isr9
global _isr10
global _isr11
global _isr12
global _isr13
global _isr14
global _isr15
global _isr16
global _isr17
global _isr18


; save cpu state
_isr_common:
    xchg bx, bx

    pusha ; save gp registers

    call interrupt_handler
    ; now we have finished handling the interrupt, restore state.
    popa
    add esp, 8  ; removes error codes and pushed isr number
    sti     ; set interrupts flag
    iret    ; pops cs, eip, eflags, ss, and esp.


; // Processor Exceptions // - occurs when the cpu can no longer resume normal execution

; _isrN:
;   cli - disable interrupts
;   push byte 0 - (only enter if cpu doesn't already push value onto the stack)
;   push byte interrupt_number 

; division by 0
_isr0:
    cli
    push byte 0
    push byte 0
    jmp _isr_common

; debug exception
_isr1:
    cli
    push byte 0 
    push byte 1
    jmp _isr_common

; non maskable interrupt exception
_isr2:
    cli 
    push byte 0
    push byte 2 
    jmp _isr_common

_isr3:
    cli 
    push byte 0
    push byte 3 
    jmp _isr_common

_isr4:
    cli
    push byte 0
    push byte 4 
    jmp _isr_common

_isr5:
    cli
    push byte 0
    push byte 5 
    jmp _isr_common

_isr6:
    cli
    push byte 0
    push byte 6
    jmp _isr_common

_isr7:
    cli
    push byte 0
    push byte 7 
    jmp _isr_common

_isr8:
    cli
    ; cpu has already pushed error code onto the stack
    push byte 8
    jmp _isr_common

_isr9:
    cli
    ; cpu has already pushed error code onto the stack
    push byte 0
    push byte 9
    jmp _isr_common

_isr10:
    cli
    push byte 10
    jmp _isr_common

_isr11:
    cli
    push byte 11
    jmp _isr_common

_isr12:
    cli
    push byte 12
    jmp _isr_common

_isr13:
    cli
    push byte 13
    jmp _isr_common

_isr14:
    cli
    push byte 14
    jmp _isr_common

_isr15:
    cli
    push 0
    push byte 15
    jmp _isr_common

_isr16:
    cli
    push 0
    push byte 16
    jmp _isr_common

_isr17:
    cli
    push 0
    push byte 17
    jmp _isr_common

_isr18:
    cli
    push 0
    push byte 18
    jmp _isr_common