#include "../drivers/vga.h"
#include "string.h"
#include "../cpu/idt.h"
#include "../drivers/driver_entry.h"

int main() {
	// TODO master driver file which intiates everything and is called by kernel.
	clear_screen();
	puts("Micah's OS");
	puts("Testing this out");
	init_idt();
	puts(">Initialised IDT");
	__asm__ volatile ("sti");
	puts(">Enabled interrupts");
	init_drivers();
	
	while (1);
}
		
