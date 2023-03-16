#include "../drivers/vga.h"
#include "string.h"
#include "../cpu/idt.h"
#include "../drivers/driver_entry.h"
#include "vmm.h"
#include "kmalloc/kmalloc.h"
#include "../util/strmap.h"


extern void _enable_paging(void);

void kmain(void) {
	clear_screen();

	vmm_init();
	_enable_paging();

	kputs("Micah's OS");
	
	init_idt();
	init_drivers();

    char* s = kmalloc(1);


	StrMap *sm;
	char buf[255];
	int result;

	sm = sm_new(10);

	if (sm == NULL) {
		/* Handle allocation failure... */
		kputs("Allocation failure...");
	}
	/* Insert a couple of string associations */
	sm_put(sm, "application name", "Testing this map");
	sm_put(sm, "application version", "1.0.0");
	/* Retrieve a value */

	result = sm_get(sm, "application name", buf, sizeof(buf));

	klog("%s hello %s %i %i", "another", "world", 2, 5000);
	klog("result: %i, value: %s", result, buf);


	if (result == 0) {
		kputs("failed...");
	}


	


}