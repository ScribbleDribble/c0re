
#include "scheduler.h"
#include "../user/surface.h"

// prototype code to test multitasking 

static uint16_t current_pid = 0;

static uint16_t n_procs = 0;

uint32_t target_esp0 = 0;

pcb_t* procs[250];
// uint16_t procs[250]; 

pcb_t* schedule(registers_t* context) {

    if (n_procs == 0) {
        procs[n_procs++] = init_process_management(context);
        procs[n_procs++] = process_clone(procs[0], n_procs, userspace_test2);
    }

    current_pid = !current_pid;
    context_switch(procs[!current_pid], procs[current_pid]);
    target_esp0 = procs[current_pid]->esp0;
    klog("target esp0 0x%x PCB phased out: 0x%x", target_esp0, procs[!current_pid]);
    // __asm__ volatile("xchg %bx, %bx");
    return procs[current_pid];
}   

// save kernel stack of the source process
void kstack_save(uint32_t new_esp0) {
    update_esp0(procs[!current_pid], new_esp0);
}


