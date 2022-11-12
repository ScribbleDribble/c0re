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
global _isr19
global _isr20
global _isr21
global _isr22
global _isr23
global _isr24
global _isr25
global _isr26
global _isr27
global _isr28
global _isr29
global _isr30
global _isr31



; save cpu state
_isr_common:
    pusha ; save gp registers
   
    pushad ; save extended registers

    mov ax, 0x10

    call interrupt_handler
    ; now we have finished handling the interrupt, restore state.
    popad

    popa
    add esp, 8  ; removes error codes and pushed isr number
    sti     ; set interrupts flag
    iret    ; pops cs, eip, eflags, ss, and esp.

    mov ax, 0x10

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
    push byte 0
    push byte 2 
    jmp _isr_common


_isr8:
    ; cpu has already pushed error code onto the stack
    push byte 8
    jmp _isr_common
