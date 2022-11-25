#include "port_io.h"
#include "irq.h"
#include "types.h"


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


int interrupt_handlers[256];


void register_interrupt_handler(uint8_t index, void (*handler)) {
    interrupt_handlers[index] = handler;
}

 
void irq_handler(uint8_t int_no) {
    char reg_value[32];
    int_to_str(int_no, reg_value, 32);
    puts(reg_value);

    if (int_no >= PIC_SECONDARY_START_INDEX)
    {
        port_byte_write(PIC_SECONDARY_CMD_PORT, PIC_SUCCESS_CODE); // send signal to secondary pic that we have handled irq
    }
    port_byte_write(PIC_MAIN_CMD_PORT, PIC_SUCCESS_CODE);

    if (interrupt_handlers[int_no] != 0) {

        void (*handler) = &(interrupt_handlers[int_no]);
        (*handler); // handler(registers);
    }
    puts("Hardware interrupt occured - system halted.");
}
