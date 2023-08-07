#include "process.h"
#include "../common/types.h"
#include "../common/debug.h"

// todo make these configurable at compile time. need to have completely separate regions of memory and page level protections of kernel stacks 
const int KSTACK_BASE = 0x30190000;
// const int USTACK_BASE = 0x90000;
const int USTACK_BASE = 0x30160000;



pcb_t* init_process_management(const irq_registers_t* registers) {
    return create_pcb_from_context(0, registers);
}


uint32_t update_pcb_and_tss_for_ctx_switch(pcb_t* src_pcb, pcb_t* dest_pcb) {
    
    tss_entry.esp0 = dest_pcb->esp0;
    dest_pcb->state = RUNNING;
    src_pcb->state = WAITING;

}

// to be called for the first process
pcb_t* create_pcb_from_context(const uint8_t pid, const irq_registers_t* context) {
    pcb_t* pcb = (pcb_t*)kmalloc(sizeof(pcb_t));
    pcb->state = RUNNING;
    pcb->pid = pid;
    pcb->esp0 = KSTACK_BASE;

    return pcb;
}

pcb_t* process_clone(pcb_t* src_pcb, int n_procs, irq_registers_t* context) {

    pcb_t* new_pcb = (pcb_t*) kmalloc(sizeof(pcb_t));    
    memory_set(new_pcb, 0, sizeof(new_pcb));
    new_pcb->state = WAITING;

    new_pcb->esp0 = KSTACK_BASE + n_procs * 0x1000;

    new_pcb->pid = n_procs;
    _setup_task(USTACK_BASE + n_procs * 0x1000, new_pcb->esp0, context->EIP); // TODO use EIP for custom entry point
    transfer_context(new_pcb->esp0, context);
    new_pcb->esp0 -= (20 + 32); // account for pusha command 
    return new_pcb;
}

void transfer_context(uint32_t kstack_base, irq_registers_t* context) {
    // helper function to copy context into new kernel stack 
    const int n_pusha_regs = 8;
    const int context_offset_bytes = 5 * MEMORY_WORD_BYTES;
    memory_copy((uint32_t*) (kstack_base - context_offset_bytes - n_pusha_regs*sizeof(int)), context, n_pusha_regs*sizeof(int));
}

void pcb_update_esp0(pcb_t* pcb, uint32_t new_esp0) {
    // TODO - should be a fixed value to prevent a falling stack
    pcb->esp0 = new_esp0;
}

