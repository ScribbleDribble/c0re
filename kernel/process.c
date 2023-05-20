#include "process.h"

const int KSTACK_BASE = 0x70000;

static bool kstack_bitmap[MAX_PROCESSES_COUNT];

pcb_t* init_process_management(const registers) {
    memory_set(kstack_bitmap, false, sizeof(kstack_bitmap));
    return create_pcb_from_context(0, registers);
}


uint32_t update_pcb_and_tss_for_ctx_switch(pcb_t* src_pcb, pcb_t* dest_pcb) {
    
    tss_entry.esp0 = dest_pcb->esp0;
    dest_pcb->state = RUNNING;
    src_pcb->state = WAITING;

}

// to be called for the first process
pcb_t* create_pcb_from_context(const uint8_t pid, const registers_t* context) {
    pcb_t* pcb = (pcb_t*)kmalloc(sizeof(pcb_t));
    pcb->state = RUNNING;
    pcb->pid = pid;
    pcb->esp0 = KSTACK_BASE;
    kstack_bitmap[0] = true;

    return pcb;
}

pcb_t* process_clone(pcb_t* src_pcb, int n_procs, uint32_t func) {

    pcb_t* new_pcb = (pcb_t*) kmalloc(sizeof(pcb_t));
    memory_set(new_pcb, 0, sizeof(new_pcb));
    new_pcb->state = WAITING;

    int i;
    for(i = 0; i < MAX_PROCESSES_COUNT; i++) {
        if (kstack_bitmap[i] == false)
        {
            kstack_bitmap[i] = true;
            new_pcb->esp0 = KSTACK_BASE + i * 0x1000;
            break;
        }
    }

    if (new_pcb->esp0 == 0) {
        klog("[sys-process]: Could not allocate kernel stack for process with pid: %i", n_procs);
        while(1);
    }
    new_pcb->esp0 = n_procs*0x1000 + src_pcb->esp0;


    new_pcb->pid = n_procs;
    _setup_task(new_pcb->esp0);
    new_pcb->esp0 -= (20 + 32); // account for pusha command 
    return new_pcb;

}

void update_esp0(pcb_t* pcb, uint32_t new_esp0) {
    pcb->esp0 = new_esp0;
}

