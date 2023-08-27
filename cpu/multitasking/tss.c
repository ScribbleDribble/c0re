#include "tss.h"

tss_descriptor_t tss_descriptor;
tss_t tss_entry;

extern KSTACK_BASE;

static uint32_t get_esp() {
	uint32_t sp;
	__asm__("mov %%esp, %%eax" : "=eax" (sp));
	return sp;
}

void create_tss() {
    memory_set(&tss_entry, 0, sizeof(tss_t));

    tss_entry.ss0 = KERNEL_DATA_SEGMENT_SELECTOR;
    tss_entry.esp0 = KSTACK_BASE;
    tss_entry.iopb = sizeof(tss_t);


}
