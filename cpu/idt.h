#include "irq.h"
#include "isr.h"


typedef struct idt_entry {
	uint16_t isr_low_offset; // bits 0..15 - low 16 bits of isr address
	uint16_t gdt_code_selector; // bits 16..31 - points to a valid code segment within gdt
	uint8_t reserved_byte; // bits 32..39 null byte
	uint8_t flags;
	uint16_t isr_high_offset; // bits 48..63
	
} __attribute__((packed))
idt_entry_t; // tell compiler to keep this arrangement of attributes when instantiated in memory.


// similar to gtd_descriptor, we provide this data to a special load command that will read this meta info for our DS
typedef struct idt_descriptor {
	uint16_t limit;
	uint32_t base_address;
}__attribute__((packed))
idt_descriptor_t;

void idt_init();




