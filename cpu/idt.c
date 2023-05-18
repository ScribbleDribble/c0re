#include "idt.h"


idt_entry_t idt[256];
idt_descriptor_t idt_descriptor;


// typedef struct register_state {
//     uint32_t eax, esi, ebp;
//     uint32_t esp, ebx, edx;
//     uint32_t ecx, edi;

// }register_state_t;


uint32_t cr2_value = 0;
extern _get_cr2_value();


typedef struct interrupt_state_t {
    int no;
    int err_code;
}interrupt_state_t;


void interrupt_handler(registers_t regs, interrupt_state_t int_state) {

    // klog("Interrupt handler was called! Err no: 0x%x", int_state.no); 

    // clean up late using func ptrs 
    // if (int_no == ISR14) {
    //     mem_map(cr2_value);
    // }
    
}

void init_idt() {
	idt_descriptor.limit = sizeof(idt_entry_t)*256 - 1; 
    idt_descriptor.base_address = (uint32_t) &idt;

    memory_set(&idt, 0, sizeof(idt));
    // maybe put in isr.c and call here. 

    // 0x8E typically used as flags - (present=1, dpl=0b00, type=0b1110 => flags=0b1000_1110=0x8E)
    // set gdt selector as 0x08 so interrupts can run kernel code segment 
    add_idt_gate(ISR0, (unsigned) _isr0, IDT_GATE_FLAGS, 0x08);
    add_idt_gate(ISR1, (unsigned) _isr1, IDT_GATE_FLAGS, 0x08);
    add_idt_gate(ISR2, (unsigned) _isr2, IDT_GATE_FLAGS, 0x08);
    add_idt_gate(ISR3, (unsigned) _isr3, IDT_GATE_FLAGS, 0x08);
    add_idt_gate(ISR4, (unsigned) _isr4, IDT_GATE_FLAGS, 0x08);
    add_idt_gate(ISR5, (unsigned) _isr5, IDT_GATE_FLAGS, 0x08);
    add_idt_gate(ISR6, (unsigned) _isr6, IDT_GATE_FLAGS, 0x08);
    add_idt_gate(ISR7, (unsigned) _isr7, IDT_GATE_FLAGS, 0x08);
    add_idt_gate(ISR8, (unsigned) _isr8, IDT_GATE_FLAGS, 0x08);
    add_idt_gate(ISR9, (unsigned) _isr9, IDT_GATE_FLAGS, 0x08);
    add_idt_gate(ISR10, (unsigned) _isr10, IDT_GATE_FLAGS, 0x08);
    add_idt_gate(ISR11, (unsigned) _isr11, IDT_GATE_FLAGS, 0x08);
    add_idt_gate(ISR12, (unsigned) _isr12, IDT_GATE_FLAGS, 0x08);
    add_idt_gate(ISR13, (unsigned) _isr13, IDT_GATE_FLAGS, 0x08);
    add_idt_gate(ISR14, (unsigned) _isr14, IDT_GATE_FLAGS, 0x08);
    add_idt_gate(ISR15, (unsigned) _isr15, IDT_GATE_FLAGS, 0x08);
    add_idt_gate(ISR16, (unsigned) _isr16, IDT_GATE_FLAGS, 0x08);
    add_idt_gate(ISR17, (unsigned) _isr17, IDT_GATE_FLAGS, 0x08);
    add_idt_gate(ISR18, (unsigned) _isr18, IDT_GATE_FLAGS, 0x08);

    // ... to implement the other 31 idt gates for processor exceptions

    // hardware interrupts - TODO maybe put this in irq.c and call here
    irq_remap();
    add_idt_gate(IRQ0, (uint32_t) _irq0, IDT_GATE_FLAGS, 0x08);
    add_idt_gate(IRQ1, (uint32_t) _irq1, IDT_GATE_FLAGS, 0x08);

    _idt_load();
    klog(">Initialised IDT, IDTR = %x", &idt_descriptor);

    __asm__ volatile ("sti");
    klog(">Enabled interrupts");
}

void add_idt_gate(uint8_t idx, uint32_t isr_offset, uint8_t flags, uint16_t gdt_code_selector) {
    int mask = 0x0000ffff;
    idt[idx].isr_low_offset = 0xffff & isr_offset;
    idt[idx].isr_high_offset = (isr_offset >> 16) & 0xffff;
    idt[idx].flags = flags;
    idt[idx].gdt_code_selector = gdt_code_selector;
}
