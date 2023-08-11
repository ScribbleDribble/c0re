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

void kmain(uint32_t* boot_page_dir, uint32_t* boot_page_table) {
	clear_screen();
	init_gdt();
	vmm_init(boot_page_dir, boot_page_table);
	klog("Micah's OS");

	init_idt();

	init_drivers(); 

	_tss_load();

	_enable_syscall();

	// allocate 4MB of memory for process 0. TODO create a vmm.c func to wrap this logic 
	palloc(0, MAX_PTE_COUNT, 0x7);
	set_pde_perms(0, 0x7);

	klog("boot_page_dir 0x%x contains 0x%x", (uint32_t)boot_page_dir, (uint32_t)*(boot_page_dir));
	klog("boot_page_dir 0x%x contains 0x%x", (uint32_t)boot_page_dir+4, (uint32_t)*(boot_page_dir+4));

	switch_to_userspace();	



	// clone_page_structures(0, 1);
	// clone_page_structures(1, 2);
	// clone_page_structures(1, 4);
	// clone_page_structures(4, 3);

	while (1);
}