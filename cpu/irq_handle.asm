global _irq0
extern irq_handler

_irq0:
    cli
    push byte 0
    push byte 32
    jmp _irq_common_stub
    

; TODO implement the other irqs

_irq_common_stub:
    pusha   ; regular register save
    pushad  ; ext reg save

    mov ax, 10
    call irq_handler

    popa
    popad
    sti

    add esp, 8
    iret ; return to code executed before.