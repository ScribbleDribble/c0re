#include "tss.h"


static uint32_t get_esp() {
	uint32_t sp;
	__asm__("mov %%esp, %%eax" : "=eax" (sp));
	return sp;
}

void create_tss() {
    memory_set(&tss_entry, 0, sizeof(tss_t));


    tss_entry.ss0 = KERNEL_DATA_SEGMENT_SELECTOR;
    tss_entry.esp0 = get_esp();
    tss_entry.iopb = sizeof(tss_t);


}
