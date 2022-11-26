#include "ps2.h"



void ps2_init() {

    // disable devices so that incoming data does not mess up intialisation
    port_byte_write(CMD_PORT, DISABLE_PORT1);
    port_byte_write(CMD_PORT, DISABLE_PORT2);

    // flush buffer by performing read
    uint8_t data = port_byte_read(DATA_PORT);
    uint8_t status_flag = port_byte_read(STATUS_PORT);
    // check to see if first bit of status flag is set to 0 to see if buf is free. 
    // keep on performing reads until to flush buffer 
    while ((status_flag & 1)) {
        puts("Flushing ps/2 output port");
        uint8_t data = port_byte_read(DATA_PORT);
    }
    char str[32];
    memory_set(str, 0, 2);
    // int_to_hex_str(status_flag, str, 32);
    // puts("status port read:");
    // puts(str);
    
    data = port_byte_read(CONFIG_REGISTER_PORT);
    
    // int i;
    // for (i = 0; i < 255; i++)
    
    //     if (data == i)
    //         int_to_hex_str(i, str, 32);

    puts("Config reg read:");
    puts(str);

    
}