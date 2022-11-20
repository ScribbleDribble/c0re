#define PIC_MAIN_START_INDEX 32
#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39

#define PIC_SECONDARY_START_INDEX 40
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

// port mapped io
#define PIC_MAIN_CMD_PORT 0x20
#define PIC_MAIN_DATA_PORT 0x21
#define PIC_SECONDARY_CMD_PORT 0xA0
#define PIC_SECONDARY_DATA_PORT 0xA1
#define PIC_RESET_CMD 0x11 
#define PIC_SUCCESS_CODE 0x20
#define PIC_IRQ_COUNT 8

// irq - interrupt generated by hardware to request for cpu time/attention 
// PIC (programmable interrupt controller) sends interrupt signals
// to cpu. 

// 2 PICs - main and secondary. Each have 8 slots, allowing them to recv 8 signals
// secondary pic IRQs (9-15) will send signal to main PIC on its 2 IRQ slot.
// The main PIC containing IRQs (0-8) is directly connected to the CPU.
// note that when IRQ 9-15 is fired, IRQ 2 will also be fired. 

// the cpu will then try to handle the irq with an appropriate isr and then
// it will write a value back into the PIC's cmd register (0x20) signalling that the operation
// was a success.

// used to declare an external function defined in irq.asm 
extern void _irq0();

void irq_remap();
