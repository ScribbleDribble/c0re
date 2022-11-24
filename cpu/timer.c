#include "timer.h"

static unsigned int tick = 0;

static void timer_callback(registers_t regs) {
    tick++;
    puts("Tick");
}

void init_timer(unsigned int frequency) {
    register_interrupt_handler(IRQ0, &timer_callback);
    
    // divisor helps define the rate at which we receive irq signals from the PIT. 
    // e.g. if we want 0.01s per interrupt then we set f=100  (since 1/100=0.01)
    // we send the divisor of the input clock and frequency. this tells us the intervals
    // at which to send signals on. must be at most 16 bits
    unsigned int divisor = INPUT_CLOCK / frequency;

    // repeat when divisor counter is 0
    port_byte_write(PIT_CMD_PORT, REPEAT_CMD);
    
    uint8_t lo = divisor & 0b11111111;
    uint8_t hi = (divisor >> 8) & 0b11111111;
    // the data ports read a byte at a time. 
    port_byte_write(PIT_DATA_PORT, lo);
    port_byte_write(PIT_DATA_PORT, hi);

}