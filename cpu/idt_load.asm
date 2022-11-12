global _idt_load
extern idt_descriptor

_idt_load:
    lidt [idt_descriptor]
    ret
