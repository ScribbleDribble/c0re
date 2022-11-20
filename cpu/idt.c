#include "idt.h"
#include "irq.h"

idt_entry_t idt[256];
idt_descriptor_t idt_descriptor;

extern void interrupt_handler() {
    puts("Interrupt handler was called!");
}

void init_idt() {
	idt_descriptor.limit = sizeof(idt_entry_t)*256 - 1; 
    idt_descriptor.base_address = &idt;

    memset(&idt, 0, 256);
    
    // maybe put in isr.c and call here. 

    // 0x8E typically used as flags - (present=1, dpl=0b00, type=0b1110 => flags=0b1000_1110=0x8E)
    // gdt_code_selector 0x08, remember each gdt segment is 8 bytes
    // first gdt segment is null and the second is our code segment.  
    add_idt_gate(0, (unsigned) _isr0, 0x8E, 0x08);
    // ... to implement the other 31 idt gates for processor exceptions

    // hardware interrupts - TODO maybe put this in irq.c and call here
    irq_remap();
    add_idt_gate(32, _irq0, 0x8E, 0x08);
    
	puts(">Initialised IDT");

    _idt_load();
}

void add_idt_gate(uint8_t idx, uint32_t isr_offset, uint8_t flags, uint16_t gdt_code_selector) {
    int mask = 0x0000ffff;
    idt[idx].isr_low_offset = mask & isr_offset;
    idt[idx].isr_high_offset = (isr_offset >> 16) & mask;
    idt[idx].flags = flags;
    idt[idx].gdt_code_selector = gdt_code_selector;
}
