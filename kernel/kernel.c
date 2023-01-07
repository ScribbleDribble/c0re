#include "../drivers/vga.h"
#include "string.h"
#include "../cpu/idt.h"
#include "../drivers/driver_entry.h"
#include "vmm.h"

extern void _enable_paging(void);
extern uint32_t* page_table;
extern uint32_t* page_directory;

#define DEBUG

void kmain(void) {

	vmm_init();
	_enable_paging();
	kputs("Paging has been enabled!");

	clear_screen();

	kputs("Micah's OS");
	
	init_idt();

	kputs(">Initialised IDT");

	__asm__ volatile ("sti");
	kputs(">Enabled interrupts");
	init_drivers();

}
		
