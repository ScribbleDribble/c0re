#include "tss.h"

tss_t* tss_entry = (tss_t*) 0x8000;

static uint32_t get_esp() {
	uint32_t sp;
	__asm__("mov %%esp, %%eax" : "=eax" (sp));
	return sp;
}

void create_tss() {

    // create tss descriptor for gdt 
    // tss_descriptor.limit = sizeof(tss_t);
    // tss_descriptor.lower_base = (base) & 0xffff;
    // tss_descriptor.mid_base = ((base) & 0xff0000) >> 16;
    // tss_descriptor.access = ACCESS;
    // tss_descriptor.flags_and_limit = 0;
    // tss_descriptor.upper_base = ((base) & 0xff000000) >> 24;

    // tss_descriptor = kmalloc(sizeof(tss_t));
    memory_set(tss_entry, 0, sizeof(tss_t));

    tss_entry->ss0 = KERNEL_DATA_SEGMENT_SELECTOR;
    tss_entry->esp0 = get_esp();
    tss_entry->iopb = sizeof(tss_t);


}
