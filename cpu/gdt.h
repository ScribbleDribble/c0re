#ifndef _GDT_H
#define _GDT_H
#include <stdint.h>


typedef struct gdt_entry_t {
    uint16_t low_limit;
    uint16_t low_base;
    uint8_t mid_base;
    uint8_t access;
    uint8_t flags_with_upper_limit;
    uint8_t upper_base;
} __attribute__((packed)) gdt_entry_t;


typedef struct gdt_descriptor_t {
    uint16_t size;
    uint32_t start;
} __attribute__((packed)) gdt_descriptor_t;

typedef struct gdt_entry_struct
{
   uint16_t limit_low;           // The lower 16 bits of the limit.
   uint16_t base_low;            // The lower 16 bits of the base.
   uint8_t  base_middle;         // The next 8 bits of the base.
   uint8_t  access;              // Access flags, determine what ring this segment can be used in.
   uint8_t  granularity;
   uint8_t  base_high;           // The last 8 bits of the base.
} __attribute__((packed)) gdt_entry_struct;

void init_gdt(void);
gdt_entry_t create_gdt_entry(uint32_t, uint32_t, uint8_t, uint8_t);

#endif