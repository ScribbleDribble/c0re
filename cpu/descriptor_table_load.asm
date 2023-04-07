global _idt_load
global _gdt_load

extern gdt_descriptor
extern idt_descriptor


_idt_load:
    lidt [idt_descriptor]
    ret

_gdt_load:
    lgdt [gdt_descriptor]
    jmp 0x08:_gdt_flush   

_gdt_flush:
    mov ax, 0x10
    mov ds, ax       
   mov es, ax
   mov fs, ax
   mov gs, ax
   mov ss, ax 
    ret
   