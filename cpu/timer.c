#include "timer.h"


#define SUCCESS_CODE 0
#define FAILURE_CODE 1


unsigned long int global_tick_count = 0;


static uint16_t get_gs(void) {
	uint32_t gs;
	__asm__("mov %%gs, %%eax" : "=eax" (gs));
	return gs;
}


void timer_callback(registers_t* regs, interrupt_state_t* int_state) {
    global_tick_count++;
    if (get_gs() == USER_DATA_SEGMENT)
    {
        schedule(regs, int_state);
    }
}

int configure_timer(unsigned int frequency) {

    if (frequency == 0) {
        kputs("[sys-timer]: Cannot divide input clock by zero. Halting OS!");
        while (1);
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