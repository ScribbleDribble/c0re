#include "gdt.h"
#include "multitasking/tss.h"
#include "../kernel/string.h"

gdt_entry_t create_gdt_entry(uint32_t, uint32_t, uint8_t, uint8_t);

gdt_entry_t gdt_entries[6]; 
gdt_descriptor_t gdt_descriptor;

extern tss_t tss_entry;
extern _gdt_load(void);
extern _tss_load(void);

void init_gdt() {

    memory_set((void*) gdt_entries, 0, sizeof(gdt_entries));
    gdt_entries[0] = create_gdt_entry(0,0,0,0);
    gdt_entries[1] = create_gdt_entry(0, 0xfffff, 0x9a, 0xc);
    gdt_entries[2] = create_gdt_entry(0, 0xfffff, 0x92, 0xc);
    gdt_entries[3] = create_gdt_entry(0, 0xfffff, 0xfa, 0xc);
    gdt_entries[4] = create_gdt_entry(0, 0xfffff, 0xf2, 0xc);

    create_tss();
    gdt_entries[5] = create_gdt_entry((uint32_t)&tss_entry, (uint32_t) sizeof(tss_t), 0x89, 0x40);
    gdt_descriptor.size = (sizeof(gdt_entry_t)*6)-1;
    gdt_descriptor.start = (uint32_t) &gdt_entries;

    _gdt_load();
    
}


gdt_entry_t create_gdt_entry(
    uint32_t base, 
    uint32_t limit, 
    uint8_t access, 
    uint8_t flags
){

    gdt_entry_t gdt_entry;

    gdt_entry.low_base =  (base & 0xffff);
    gdt_entry.low_limit =  (limit & 0xffff);
    gdt_entry.mid_base =  ((base & 0x00ff0000) >> 16);
    gdt_entry.access = access; 
    gdt_entry.flags_with_upper_limit =  (flags << 4) | ((limit & 0xf0000) >> 16);
    gdt_entry.upper_base =  (base & 0xff000000) >> 24;

    return gdt_entry;
}


