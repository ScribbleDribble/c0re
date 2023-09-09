#ifndef _PROCESS_H
#define _PROCESS_H

#include <stdint.h>
#include "../cpu/multitasking/tss.h"
#include "kmalloc/kmalloc.h"
#include "../cpu/irq.h"
#include "string.h"
#include "memory_defs.h"

#define MAX_PROCESSES_COUNT 256
// each process to have 
// - 4kb kernel and user stack
// TODO guard pages for each kernel stack 

enum ProcessState {FINISHED = 0, RUNNING = 1, WAITING = 2, READY = 3, NEW = 4};

typedef struct pcb_t {
    uint32_t esp0;
    uint16_t pid; 
    uint32_t ebp;
    uint32_t* read_only_pd;
    enum ProcessState state;

}__attribute__((packed)) pcb_t;

extern void _setup_task(uint32_t esp3, uint32_t esp0, uint32_t eip);

uint32_t context_switch(pcb_t* src_pcb, pcb_t* dest_pcb);
pcb_t* create_pcb_from_context(const uint8_t pid, const registers_t* context);
pcb_t* process_clone(pcb_t* src_pcb, int n_procs, const registers_t* context, interrupt_state_t* int_state);
void pcb_update_esp0(pcb_t* pcb, uint32_t new_esp0);

#endif 