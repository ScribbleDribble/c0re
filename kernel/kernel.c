#include "../drivers/vga.h"
#include "string.h"
#include "../cpu/idt.h"
#include "../drivers/driver_entry.h"
#include "vmm.h"
#include "kmalloc/kmalloc.h"


extern void _enable_paging(void);

void kmain(void) {
	clear_screen();

	vmm_init();
	_enable_paging();

	kputs("Micah's OS");
	
	init_idt();
	init_drivers();


	// terminal bug to fix 
	// if something is printed 5 times in a row, it will then appear on each line
	int i;
	for (i = 0; i < 500; i++) {
		kmalloc(i);
	}

	// heap example
	// int i;
	// char* s;
	// clear_screen();




}