#include "../drivers/vga.h"
#include "string.h"
#include "../cpu/idt.h"
#include "../drivers/driver_entry.h"
#include "vmm.h"

extern void _enable_paging();

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

	char buf[16];
	int i;
	for (i = 0; i < 10; i++) {
		int_to_str(i, buf, 10);
		kputs(buf);
		memory_set(buf, 0, 16);

	}
	// vmm_init();
	// _enable_paging();
	// kputs("Paging has been enabled!");

	while (1);
}
		
