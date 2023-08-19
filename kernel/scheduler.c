
#include "scheduler.h"

// prototype code to test multitasking 

uint8_t current_pid = 0;

uint16_t prev_pid = 0;

static uint16_t n_procs = 0;

uint32_t target_esp0 = 0;

pcb_t* procs[250];

pcb_t* schedule(const registers_t* context, interrupt_state_t* int_state) {
    if (n_procs == 0) {
        procs[n_procs++] = init_process_management(context);
        procs[n_procs++] = process_clone(procs[0], n_procs, context, int_state);
    }

    current_pid++;
    if (current_pid == n_procs) {
        current_pid = 0;
        prev_pid = n_procs-1;
    } else {
        prev_pid = current_pid-1;
    }

    update_pcb_and_tss_for_ctx_switch(procs[prev_pid], procs[current_pid]);
    target_esp0 = procs[current_pid]->esp0;
    klog("source esp0: 0x%x | target esp0 0x%x |\ncurrent pid: %i", procs[prev_pid]->esp0, procs[current_pid]->esp0, current_pid);
    asm("xchg %bx, %bx");
    return procs[current_pid];
}   

// save kernel stack of the source process
void kstack_save(uint32_t new_esp0) {
    if (procs[!current_pid]->state == RUNNING) {
        klog("[Process] Should not perform ESP0 save on a running process!");
    }
    klog("Saving stack for process pid: %i", procs[prev_pid]->pid);
    pcb_update_esp0(procs[prev_pid], new_esp0);
}


