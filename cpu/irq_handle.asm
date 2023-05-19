global _irq0
global _irq1

extern process_hardware_interrupt
extern target_esp0
extern userspace_test2
extern ctx_switch

_irq0:
    cli
    push byte 0
    push byte 32
    jmp _irq_common_stub

_irq1:
    cli 
    push byte 0
    push byte 33
    jmp _irq_common_stub
    

; TODO implement the other irqs

_irq_common_stub:
	; xchg bx, bx

    push esp
    pusha   ; regular register save
    ; pushad  ; ext reg save
    mov ax, 10

    call process_hardware_interrupt
    ; xchg bx, bx

    popa  
    pop esp
    ; popad


    push eax 
    mov eax, [target_esp0]
    cmp eax, 0
    ; clean up stack for iret 
    pop eax
    
    jne _finalise_context_switch
    add esp, 8

    sti
    iret ; the special kind of ret for interrupts


; sets eip to newly running process 
_finalise_context_switch:
    add esp, 8
    jmp ctx_switch
    
