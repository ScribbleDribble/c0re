#include "process.h"
#include "../common/types.h"
#include "../common/debug.h"

extern tss_t tss_entry;

// todo make these configurable at compile time. need to have completely separate regions of memory and page level protections of kernel stacks 
const int KSTACK_PAGE_OFFSET = 0x100000;
const int KSTACK_BASE = (PD_IDX_TO_ADDRESS(KERNEL_STACK_PD_IDX) + KSTACK_PAGE_OFFSET);
const int USTACK_BASE = 0x90000;



pcb_t* init_process_management(const registers_t* registers) {
    palloc(KERNEL_STACK_PD_IDX, MAX_PTE_COUNT, PAGE_P | PAGE_W);
    create_pcb_from_context(0, registers);
}


uint32_t update_pcb_and_tss_for_ctx_switch(pcb_t* src_pcb, pcb_t* dest_pcb) {
    tss_entry.esp0 = dest_pcb->esp0;
    dest_pcb->state = RUNNING;

    if (src_pcb->state != WAITING) {
        src_pcb->state = READY;
    }
}

// to be called for the first process
pcb_t* create_pcb_from_context(const uint8_t pid, const registers_t* context) {
    pcb_t* pcb = (pcb_t*)kmalloc(sizeof(pcb_t));
    pcb->waiting_params = (waiting_params_t*)kmalloc(sizeof(waiting_params_t));
    pcb->state = RUNNING;
    pcb->pid = pid;
    pcb->esp0 = KSTACK_BASE;

    return pcb;
}   

pcb_t* process_clone(pcb_t* src_pcb, int n_procs, const registers_t* context, interrupt_state_t* int_state) {

    pcb_t* new_pcb = (pcb_t*) kmalloc(sizeof(pcb_t));    
    new_pcb->waiting_params = (waiting_params_t*)kmalloc(sizeof(waiting_params_t));
    memory_set(new_pcb, 0, sizeof(new_pcb));
    new_pcb->state = READY;
    new_pcb->esp0 = KSTACK_BASE + n_procs * 0x1000;
    new_pcb->pid = n_procs;
    setup_kernel_stack(new_pcb->esp0, src_pcb->esp0, context, int_state);
    // new_pcb->esp0 -= (20 + 32); // account for pusha command 
    // // temporary until we implment COW
    clone_page_structures(src_pcb->pid, new_pcb->pid);
    diverge_physical_mappings(new_pcb->pid);
    return new_pcb;
}

// helper function to create stack with the following entries for iret (interrupt return)
// ss -> ESP (this should be user stack value) -> EFLAGS -> CS -> code entry point
void setup_kernel_stack(uint32_t kstack_base, uint32_t prev_kstack_base, registers_t* context, interrupt_state_t* int_state) {
    const int n_pusha_regs = 8;
    const int context_offset_bytes = 5 * sizeof(uint32_t);
    int ss = 0;
    ss = int_state->cs == 0x8 ? 0x10 : 0x20 | 0x3;
    uint32_t upper_stack_setup[] = {int_state->eip, int_state->cs, int_state->eflags, int_state->task_stack_addr, ss};
    memory_copy((uint32_t*) (kstack_base - context_offset_bytes), upper_stack_setup, sizeof(uint32_t)*5);
    // memory_copy(kstack_base-SIZEOF_KERNEL_STACK, prev_kstack_base-SIZEOF_KERNEL_STACK, SIZEOF_KERNEL_STACK);
    memory_copy((uint32_t*) (kstack_base - context_offset_bytes - n_pusha_regs*sizeof(int)), context, n_pusha_regs*sizeof(int));    // could assert stack state 
    
    int* kernel_stack_base = kstack_base;
    *kernel_stack_base = sizeof(upper_stack_setup);
}

void pcb_update_esp0(pcb_t* pcb, uint32_t new_esp0) {
    // TODO - should be a fixed value to prevent a falling stack
    pcb->esp0 = new_esp0;
}


void set_sleep(pcb_t* pcb, int expiry_tick) {
    // we are interrupting every 100ms so lets work out how many 
    pcb->waiting_params->ready_tick = expiry_tick; 
    pcb->state = WAITING;
    return;
}

void wakeup(pcb_t* pcb) {
    pcb->state = READY;
    pcb->waiting_params->ready_tick = NULL;
    klog("esp0 %x, pid: %i, read_only_pd: %x, state: %i", pcb->esp0, pcb->pid, pcb->read_only_pd, pcb->state);
}

