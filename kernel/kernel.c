#include "../drivers/vga.h"
#include "string.h"
#include "../cpu/idt.h"
#include "../drivers/driver_entry.h"
#include "vmm.h"
#include "kmalloc/kmalloc.h"
#include "../cpu/gdt.h"


extern void _enable_paging(void);
extern void switch_to_userspace(void);
extern void _simulate_gpf(void);

void kmain(void) {

	init_gdt();

	vmm_init();
	_enable_paging();
	clear_screen();
	klog("Micah's OS");

	init_idt();


	init_drivers();

	while(1);

	// list_status_logger(0, 100);

	// switch_to_userspace();
	
}