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

void kmain(void) {

	init_gdt();


	// vmm_init();
	// // _enable_paging();
	// klog("Micah's OS");

	// init_idt();

	// init_drivers(); 
	// // asm ("xchg %bx, %bx");

	// _tss_load();

	// _enable_syscall();

	// int a = 10;

	// klog("The address is: %i", (uint32_t) &a);

	// int i;
	// uint32_t* pd = (uint32_t*) PD_BASE_ADDR;

	// for (i = 0; i < 10; i++) {
	// 	klog("pde index %i with address: 0x%x with contents 0x%x", i, (uint32_t) (pd + i), pd[i]);
	// }

	// switch_to_userspace();



	while (1);
}