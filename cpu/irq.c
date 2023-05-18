#include "irq.h"


// IRQs by default have their isr mappings starting at 0 but not 32.
// this will be a problem since we are in protected mode and have
// processor interrupts taken in that slot, so we remap.
void irq_remap() {
    const int x86_arch_mode = 0x1;
    const int enable_all_irqs = 0;
    const int pic_main_connector_slot = 0x4;
    const int pic_secondary_connector_slot = 0x2;
    // send initialise command to master pic command port. 
    port_byte_write(PIC_MAIN_CMD_PORT, PIC_RESET_CMD);
    // after sending this command, the PIC now expects 4 parameters via its data port
    // 1. The first parameter is the starting offsets for the new IRQ numbers. we want to start at 32 for our main pic. 
    port_byte_write(PIC_MAIN_DATA_PORT, IRQ0);
    // 2. Specify the slot in the pic that connects to the other PIC (4th slot in master)
    port_byte_write(PIC_MAIN_DATA_PORT, pic_main_connector_slot);
    // 3. specify x86 mode
    port_byte_write(PIC_MAIN_DATA_PORT, x86_arch_mode);
    // 4. tells which IRQs to disable. we want them all enabled, so enter 0
    port_byte_write(PIC_MAIN_DATA_PORT, enable_all_irqs);

    // we repeat, but with some changes to make it relevant for secondary pic
    port_byte_write(PIC_SECONDARY_CMD_PORT, PIC_RESET_CMD);
    port_byte_write(PIC_SECONDARY_DATA_PORT, IRQ8);
    port_byte_write(PIC_SECONDARY_DATA_PORT, pic_secondary_connector_slot);
    port_byte_write(PIC_SECONDARY_DATA_PORT, x86_arch_mode);
    port_byte_write(PIC_SECONDARY_DATA_PORT, enable_all_irqs);

}

typedef void (*irq_handler_func_t) (registers_t*); // type of func ptr is just irq_handler

irq_handler_func_t interrupt_handlers[256];


void register_interrupt_handler(uint8_t index, void (*handler) (registers_t*)) {
    interrupt_handlers[index] = handler;
}

// maybe push more registers to the stack if we need the data
typedef struct interrupt_state_t {
    int no;
    int err_code;
}interrupt_state_t;


// handler for all irqs. calls handler specific to irq
void process_hardware_interrupt(registers_t regs, interrupt_state_t int_state) {
    if (int_state.no >= PIC_SECONDARY_START_INDEX)
    {
        port_byte_write(PIC_SECONDARY_CMD_PORT, PIC_SUCCESS_CODE); // send signal to secondary pic that we have handled irq
    }
    port_byte_write(PIC_MAIN_CMD_PORT, PIC_SUCCESS_CODE);

    if (interrupt_handlers[int_state.no] != 0) {
        irq_handler_func_t handler;
        handler = interrupt_handlers[int_state.no];
        handler(&regs);
    }
}
