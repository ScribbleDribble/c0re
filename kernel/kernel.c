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

	char* s = kmalloc(6);

	s = realloc(s, 4085);






}