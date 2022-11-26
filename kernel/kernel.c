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
	puts(">Enabled interrupts");


	// generates cpu exception, triggering isr0
	// int val = 5/0;

	// generates a hardware interrupt, triggering irq1
	// __asm__ volatile ("int $0x21");
	while (1) {
		// puts("OS loop");
	}
}
		
