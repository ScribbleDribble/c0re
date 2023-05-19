#include "timer.h"


#define SUCCESS_CODE 0
#define FAILURE_CODE 1


uint32_t tick = 0;

extern line;

static uint16_t get_gs() {
	uint32_t gs;
	__asm__("mov %%gs, %%eax" : "=eax" (gs));
	return gs;
}


void timer_callback(registers_t* regs) {
    tick++;
    if (get_gs() == 0x23)
    {
        pcb_t* cur_process = schedule(regs);
        // klog("Process id: %i, esp0: 0x%x, tss->esp0: 0x%x", cur_process->pid, cur_process->esp0, tss_entry.esp0);
    }
}

int configure_timer(unsigned int frequency) {

    if (frequency == 0) {
        kputs("Cannot divide input clock by zero. Halting OS!");
        return FAILURE_CODE;
    }
    
    register_interrupt_handler(IRQ0, &timer_callback);
    
    // divisor helps define the rate at which we receive irq signals from the PIT. 
    // e.g. if we want 0.01s per interrupt then we set f=100  (since 1/100=0.01)
    // we send the divisor of the input clock and frequency. this tells us the intervals
    // at which to send signals on. must be at most 16 bits
    uint32_t divisor = INPUT_CLOCK / frequency;

    // repeat when divisor counter is 0
    port_byte_write(PIT_CMD_PORT, REPEAT_CMD);
    
    char lo = divisor & 0xFF;
    char hi = (divisor & 0xFF00) >> 8;
    // the data ports read a byte at a time. 
    port_byte_write(PIT_DATA_PORT, lo);
    port_byte_write(PIT_DATA_PORT, hi);

    return SUCCESS_CODE;
}