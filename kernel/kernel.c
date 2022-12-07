#include "../drivers/vga.h"
#include "string.h"
#include "../cpu/idt.h"
#include "../drivers/driver_entry.h"

int main() {
	// TODO master driver file which intiates everything and is called by kernel.
	clear_screen();
	kputs("Micah's OS");
	kputs("Testing this out");
	init_idt();
	kputs(">Initialised IDT");
	__asm__ volatile ("sti");
	kputs(">Enabled interrupts");
	init_drivers();
	
	while (1);
}
		
