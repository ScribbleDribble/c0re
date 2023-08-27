#include "process.h"
#include "../common/types.h"
#include "../common/debug.h"

extern tss_t tss_entry;

// todo make these configurable at compile time. need to have completely separate regions of memory and page level protections of kernel stacks 
const int KSTACK_BASE = 0x30190000;
const int USTACK_BASE = 0x90000;



pcb_t* init_process_management(const registers_t* registers) {
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

    return pcb;
}

pcb_t* process_clone(pcb_t* src_pcb, int n_procs, const registers_t* context, interrupt_state_t* int_state, uint32_t src_pid) {

    pcb_t* new_pcb = (pcb_t*) kmalloc(sizeof(pcb_t));    
    memory_set(new_pcb, 0, sizeof(new_pcb));
    new_pcb->state = WAITING;
    new_pcb->esp0 = KSTACK_BASE + n_procs * 0x1000;
    new_pcb->pid = n_procs;
    setup_kernel_stack(new_pcb->esp0, context, int_state);
    new_pcb->esp0 -= (20 + 32); // account for pusha command 

    // klog("pid %i", new_pcb->pid);
    // // temporary until we implment COW
    // clone_page_structures(0, new_pcb->pid);
    // // diverge_physical_mappings(new_pcb->pid);
    // reload_cr3(1); // WIP
    // while (1);

    return new_pcb;
}

// helper function to create stack with the following entries for iret (interrupt return)
// DS -> ESP (this should be user stack value) -> EFLAGS -> CS -> code entry point
void setup_kernel_stack(uint32_t kstack_base, const registers_t* context, interrupt_state_t* int_state, uint32_t user_stack_base) {
    const int n_pusha_regs = 8;
    const int context_offset_bytes = 5 * sizeof(uint32_t);
    uint32_t upper_stack_setup[] = {int_state->eip, int_state->cs | 0x18 | 0x3, 0x202, user_stack_base, 0x20 | 0x3};
    memory_copy((uint32_t*) (kstack_base - context_offset_bytes), upper_stack_setup, sizeof(uint32_t)*5);
    memory_copy((uint32_t*) (kstack_base - context_offset_bytes - n_pusha_regs*sizeof(int)), context, n_pusha_regs*sizeof(int));
    // you can insert a breakpoint here to validate the stack sequence
}

void pcb_update_esp0(pcb_t* pcb, uint32_t new_esp0) {
    // TODO - should be a fixed value to prevent a falling stack
    pcb->esp0 = new_esp0;
}

