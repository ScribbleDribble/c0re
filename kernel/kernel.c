#include "../drivers/screen.h"
#include "string.h"
#include "../cpu/idt.h"

int main() {
	
	clear_screen();
	puts("Micah's OS");
	puts("Testing this out");
	init_idt();
	puts(">Initialised IDT");
	__asm__ volatile ("sti");
	puts("Enabled interrupts");
	// __asm__ volatile ("int $0x20");


	// generates cpu exception, triggering isr0
	int val = 5/0;

	// generates a hardware interrupt, triggering irq0
	// __asm__ volatile ("int $0x20");
	while (1) {

	}
}
		
