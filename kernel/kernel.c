#include "../drivers/vga.h"
#include "string.h"
#include "../cpu/idt.h"
#include "../drivers/ps2.h"
#include "../drivers/keyboard.h"
#include "../cpu/timer.h"

int main() {
	// TODO master driver file which intiates everything and is called by kernel.
	clear_screen();
	puts("Micah's OS");
	puts("Testing this out");
	init_idt();
	puts(">Initialised IDT");
	__asm__ volatile ("sti");
	puts(">Enabled interrupts");
	configure_timer(10);

	ps2_init();
	keyboard_init();

	// generates cpu exception, triggering isr0
	// int val = 5/0;

	// generates a hardware interrupt, triggering irq1
	// __asm__ volatile ("int $0x21");
	while (1) {
		// puts("OS loop");
	}
}
		
