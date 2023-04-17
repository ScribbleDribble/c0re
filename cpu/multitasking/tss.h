
// will be used to help us return to ring 0 from user mode. stores kernel stack segment, allowing us to execute kernel code
// as we are doing software multitasking, we are only interested in esp0 and ss0.
// when a system call is made, we retrieve ss0 and set the stack pointer to be esp0. esp = ss0:esp0
#ifndef _TSS_H
#define _TSS_H

#include <stdint.h>

#define KERNEL_DATA_SEGMENT_SELECTOR 0x10



#define ACCESS 0x89 // 

typedef struct tss_descriptor {
    uint16_t limit;
    uint16_t lower_base;
    uint8_t mid_base;
    uint8_t access;
    uint8_t flags_and_limit;
    uint8_t upper_base;
}__attribute__((packed)) tss_descriptor_t;

typedef struct tss_t {
    
	uint16_t prev_tss_selector;
	uint16_t reserved1;
	uint32_t esp0;
    uint16_t ss0;
    uint16_t reserved2;
    uint32_t esp1;
    uint16_t ss1;
	uint16_t reserved3;
    uint32_t esp2;
    uint16_t ss2;
	uint16_t reserved4;
    uint32_t cr3;
    uint32_t eip;
    uint32_t eflags;
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
    uint16_t es;
	uint16_t reserved5;
    uint16_t cs;
	uint16_t reserved6;
    uint16_t ss;
	uint16_t reserved7;
    uint16_t ds;
	uint16_t reserved8;
    uint16_t fs;
	uint16_t reserved9;
    uint16_t gs;
	uint16_t reserved10;
    uint16_t ldtr;
	uint16_t reserved11;
	uint16_t reserved12;
    uint16_t iopb;
    uint32_t ssp;
}__attribute__((packed)) tss_t;

tss_descriptor_t tss_descriptor;
tss_t tss_entry;

void _load_tss(void);

void create_tss(void);

#endif
