#include "../drivers/vga.h"
#include "string.h"
#include "../cpu/idt.h"
#include "../drivers/driver_entry.h"
#include "vmm.h"

extern void _enable_paging(void);
void kmain(void) {
	vmm_init();
	_enable_paging();

	clear_screen();
	kputs("Micah's OS");

	init_idt();
	init_drivers();

}