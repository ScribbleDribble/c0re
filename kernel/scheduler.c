
#include "scheduler.h"

extern global_tick_count;

uint8_t current_pid = 0;

uint16_t prev_pid = 0;

uint16_t n_procs = 0;

uint32_t target_esp0 = 0;

pcb_t* procs[250];

priority_queue_t* wait_queue;


int select_next_process(int next_pid) {

    int i = 0;
    while(procs[next_pid]->state != READY && procs[next_pid]->state != RUNNING ) {
        if (next_pid == n_procs) {
            next_pid = 0;
        } else {
            next_pid++;
        }
        klog("process with pid %i is not ready", next_pid, procs[next_pid]->state);
        i++;
        
        if (i > n_procs) {
            panic("No more processes available to run"); 
        }
    }
    return next_pid;
}

pcb_t* schedule(const registers_t* context, interrupt_state_t* int_state) {
    if (n_procs == 0) {
        procs[n_procs++] = init_process_management(context);
        wait_queue = register_priority_queue();
    }

    current_pid = select_next_process(current_pid+1);
    
    if (current_pid == 0) {
        prev_pid = n_procs-1;
    } else {
        prev_pid = current_pid-1;
    }
    
    // poll_waiting_processes();

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
    // TODO check if max amount of processes was reached 
    klog("state: esp %x, eax %x, edx %x, int_state: cs: %x, eflags: %x, eip %x", int_state.task_stack_addr, context.AX, context.DX, int_state.cs, int_state.eflags, int_state.eip);
    procs[n_procs++] = process_clone(procs[current_pid], n_procs, &context, &int_state);

    return procs[n_procs-1]->pid;
}

uint32_t get_running_proc_pid() {
    return current_pid;
}

void wait_process(int seconds) {
     // calculate expected tick / point where we can set this process as READY.
    // e.g. (given a tick is 100 ms) tick so we expect 10 ticks. 
    // we will use a min-heap PQ so 10 tick to be prioritised over 11 ticks

    // int milliseconds_per_tick = 100;
    // int ticks = seconds*1000 / milliseconds_per_tick;

    // pcb_t* process_pcb = procs[current_pid];
    // sleep(process_pcb, ticks + global_tick_count);
    
    // pq_insert(wait_queue, process_pcb->waiting_params->ready_tick, process_pcb);

    // TODO have a counter of ticks as global tick. this is the system clock
    // insert with ticks+global tick
    // poll until tick+global tick period has reached then unwait process
    // do wrap around check on global_tick+tick to check for integer overflow
}

void poll_waiting_processes() {
    int error = 100; // this may not be neeeded
    pcb_t* pcb = pq_peek(wait_queue);
    if (pcb == NULL || pcb->waiting_params->ready_tick != global_tick_count) 
    {
        klog("Process with PID: %i has ticks remaining",  pcb->waiting_params->ready_tick - global_tick_count);
        return;
    }
    pq_poll(wait_queue);
    wakeup(pcb);
}