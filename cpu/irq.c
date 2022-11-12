#include "port_io.h"
#include "irq.h"
#include "types.h"


// IRQs by default have their isr mappings starting at 0 but not 32.
// this will be a problem since we are in protected mode and have
// processor interrupts taken in that slot, so we remap.
void irq_remap() {
    port_byte_write(0x20, 0x11);
    port_byte_write(0x20, 0x11);
    port_byte_write(0xA0, 0x11);
    port_byte_write(0x21, 0x20);
    port_byte_write(0xA1, 0x28);
    port_byte_write(0x21, 0x04);
    port_byte_write(0xA1, 0x02);
    port_byte_write(0x21, 0x01);
    port_byte_write(0xA1, 0x01);
    port_byte_write(0x21, 0x0);
    port_byte_write(0xA1, 0x0);
}

int interrupt_handlers[256];

void irq_handler(registers_t registers) {

    if (registers.interrupt_no >= PIC_SECONDARY_START_INDEX)
    {
        port_byte_write(PIC_SECONDARY_PORT, PIC_SUCCESS_CODE); // send signal to secondary pic that we have handled irq
    }
    port_byte_write(PIC_MAIN_PORT, PIC_SUCCESS_CODE);

    if (interrupt_handlers[registers.interrupt_no] != 0) {
        // isr_t handler = interrupt_handler[registers.interrupt_handler];
        // handler(registers);
    }
    kprint("hardware interrupt occured!", 0x0f, 20, 20);
}
