global _irq0
global _irq1

extern process_hardware_interrupt

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
    ;pusha   ; regular register save
    ; pushad  ; ext reg save
    mov ax, 10
    call process_hardware_interrupt

    ;popa
    ;popad

    add esp, 8
    sti
    iret ; the special kind of ret for interrupts

