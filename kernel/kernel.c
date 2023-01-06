#include "../drivers/vga.h"
#include "string.h"
#include "../cpu/idt.h"
#include "../drivers/driver_entry.h"
#include "vmm.h"

extern void _enable_paging();
extern uint32_t* page_table;
extern uint32_t* page_directory;


int kmain() {
	// TODO master driver file which intiates everything and is called by kernel.
	vmm_init();
	_enable_paging();
	kputs("Paging has been enabled!");

	clear_screen();

	kputs("Micah's OS");
	kputs("Testing this out");

	
	init_idt();

	

	kputs(">Initialised IDT");

	__asm__ volatile ("sti");
	kputs(">Enabled interrupts");
	init_drivers();
	
	

	char buf[32];
	// should point to 0x42000 and have flags as 0x7. res = 0x00042007
	int_to_hex_str(*page_directory, buf, 32);
	kputs(buf);
	// should be address of first page table i.e 0x00042000
	int_to_hex_str(page_table, buf, 32);
	kputs(buf);

	// should point to first 4kb aligned address in memory that is free to use, provided by kalloc. should be 0x00100000. 
	// should also have flags as 0x7
	// so 0x00100007
	int_to_hex_str(*page_table, buf, 32);
	kputs(buf);
	// next entry in page table should point to the next value provided by kalloc. should be 0x00101007
	int_to_hex_str(*(page_table+1), buf, 32);
	kputs(buf);
	// last entry has a physical address, when subtracted with phys address pointed by first pte, produces 4MB difference (4190208). 
	int_to_str((uint32_t)*(page_table+1023)-*(page_table), buf, 32);
	kputs(buf);

	// last entry of page directory should point to the first page table, so its value should be 0x0004200
	int_to_hex_str(*(page_directory + 1023), buf, 32);
	kputs(buf);

}
		
