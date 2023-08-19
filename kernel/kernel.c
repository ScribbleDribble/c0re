#include "../drivers/vga.h"
#include "string.h"
#include "../cpu/idt.h"
#include "../drivers/driver_entry.h"
#include "vmm.h"
#include "kmalloc/kmalloc.h"
#include "../cpu/gdt.h"
#include "../cpu/multitasking/tss.h"
#include "../util/spinlock.h"
#include "../drivers/serial_io.h"


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

	user_space_vmm_init();

	serial_init();

	// clone_page_structures(0, 1);
	// diverge_physical_mappings(1);
	switch_to_userspace();
	
	// clone_page_structures(3, 1);
	// clone_page_structures(3, 2);
	// clone_page_structures(4, 3);
	// klog("System memory available: %i", get_total_usable_space_bytes());

	while (1);
}