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
    pusha   ; regular register save
    mov ax, 10

    call process_hardware_interrupt

    popa  

    push eax 
    mov eax, [target_esp0]
    cmp eax, 0
    ; clean up stack for iret 
    pop eax
    
    je _irq_return
    push eax 
    mov eax, [esp + 4]
    cmp eax, 32
    pop eax 

    je _begin_context_switch

    jmp _irq_return

    

_irq_return:
    add esp, 8
    sti
    iret 


; sets eip to newly running process 
_begin_context_switch:
    add esp, 8
    jmp ctx_switch
    
