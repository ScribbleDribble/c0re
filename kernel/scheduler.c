
#include "scheduler.h"

// prototype code to test multitasking 

uint8_t current_pid = 0;

uint16_t prev_pid = 0;

uint16_t n_procs = 0;

uint32_t target_esp0 = 0;

pcb_t* procs[250];

pcb_t* schedule(const registers_t* context, interrupt_state_t* int_state) {
    if (n_procs == 0) {
        procs[n_procs++] = init_process_management(context);
        // procs[n_procs++] = process_clone(procs[0], n_procs, context, int_state);
        // procs[n_procs++] = process_clone(procs[0], n_procs, context, int_state);
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
    klog("source esp0: 0x%x | target esp0 0x%x |current pid: %i", procs[prev_pid]->esp0, procs[current_pid]->esp0, current_pid);

    reload_cr3(current_pid);

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

uint32_t schedule_new_fork(const registers_t context, interrupt_state_t int_state) {
    if (n_procs == 0) {
        procs[n_procs++] = init_process_management(context);
    }
    klog("state: esp %x, eax %x, edx %x, int_state: cs: %x, eflags: %x, eip %x", int_state.task_stack_addr, context.AX, context.DX, int_state.cs, int_state.eflags, int_state.eip);
    procs[n_procs++] = process_clone(procs[current_pid], n_procs, &context, &int_state);

    return procs[n_procs-1]->pid;
}

uint32_t get_running_proc_pid() {
    return current_pid;
}

