#include "serial_io.h"


void serial_init() {

    //disable interrupts
    port_byte_write(COM1_PORT + 1, 0);
    set_baud_rate();
    // set parameters for controller 
    port_byte_write(COM1_PORT+LINE_CONTROL_REG_OFFSET, 3); // 8-bit chars, 2 stop bits, space parity bit

    port_byte_write(COM1_PORT+ 2, 0xC7); // enable fifo

    port_byte_write(COM1_PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
   port_byte_write(COM1_PORT + 4, 0x1E);    // Set in loopback mode, test the serial chip

   port_byte_write(COM1_PORT, 0xAE);

    if (port_byte_read(COM1_PORT) != 0xAE) {
        klog("Serial port err occurred!");
    }

    port_byte_write(COM1_PORT + 4, 0x0F);


    port_byte_write(COM1_PORT, 0x4D);
    port_byte_write(COM1_PORT, 0x45);

    klog("Serial init success");
}

// Baud rate should be similar to that of the CPU timer we configured, so close to 119318 baud
void set_baud_rate() {
        //f = 0.9654871855042827

    // set MSB of Line Control Register 
    port_word_write(COM1_PORT + LINE_CONTROL_REG_OFFSET, 128);
    // set least significant BYTE to port+0
    port_word_write(COM1_PORT + DLAB_ON_SET_LOW_DIVISOR_OFFSET, 1);
    // set most significant BYTE to port+1
    port_byte_write(COM1_PORT+ DLAB_ON_SET_HIGH_DIVISOR_OFFSET, 0);

    // clear last bit of line port register
    port_byte_write(COM1_PORT + LINE_CONTROL_REG_OFFSET, 0);

}

void serial_ws(char* s) {
    int i;
    for (i = 0; i < str_len(s); i++) {
        serial_write(s[i]);
    }
}

void serial_write(char data) {
    port_byte_write(COM1_PORT, (uint8_t) data);
}

char serial_read(char data) {
    return (char) port_byte_read(COM1_PORT);
}
