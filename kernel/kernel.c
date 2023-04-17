#include "../drivers/vga.h"
#include "string.h"
#include "../cpu/idt.h"
#include "../drivers/driver_entry.h"
#include "vmm.h"
#include "kmalloc/kmalloc.h"
#include "../cpu/gdt.h"
#include "../cpu/multitasking/tss.h"


extern void _enable_paging(void);
extern void switch_to_userspace(void);
extern void _simulate_gpf(void);
extern void _tss_load(void);


void kmain(void) {
	clear_screen();

	init_gdt();
	// asm volatile ("xchg %bx, %bx");

	vmm_init();
	_enable_paging();
	klog("Micah's OS");

	init_idt();

	init_drivers();

	_tss_load();

	switch_to_userspace();

	while (1);

}