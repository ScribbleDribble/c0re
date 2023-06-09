#include "../drivers/vga.h"
#include "string.h"
#include "../cpu/idt.h"
#include "../drivers/driver_entry.h"
#include "vmm.h"
#include "kmalloc/kmalloc.h"
#include "../cpu/gdt.h"
#include "../cpu/multitasking/tss.h"
#include "../util/spinlock.h"


extern void _enable_paging(void);
extern void _enable_syscall(void);
extern void switch_to_userspace(void);
extern void _tss_load(void);
extern void _invalidate_pde0(void);

void kmain(uint32_t* boot_page_dir, uint32_t* boot_page_table) {
	clear_screen();
	init_gdt();
	vmm_init(boot_page_dir, boot_page_table);
	klog("Micah's OS");

	init_idt();

	init_drivers(); 
	// // asm ("xchg %bx, %bx");

	_tss_load();

	_enable_syscall();

	switch_to_userspace();



	while (1);
}