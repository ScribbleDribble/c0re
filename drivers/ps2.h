#ifndef _PS2_H
#define _PS2_H

#include <stdint.h>

#include "../cpu/port_io.h"
#include "../kernel/string.h"
#include "../drivers/vga.h" // kputs
#include "../cpu/irq.h"     


#define CMD_PORT 0x64
#define DATA_PORT 0x60 // used for reading data that was received from a PS/2 device or from the PS/2 controller itself and writing data to a PS/2 device or to the PS/2 controller itself.
#define STATUS_PORT 0x64
#define CONFIG_REGISTER_PORT 0x20

// commands // 
#define DISABLE_PORT1 0xAD // a port is used by one ps/2 device
#define DISABLE_PORT2 0xA7
#define ENABLE_PORT1 0xAE

#define RESET_DEVICE 0xFF
#define IDENTIFY_DEVICE 0xF2

#define READ_CONTROLLER_CONFIG 0x20
#define WRITE_CONTROLLER_CONFIG 0x60
#define TEST_CONTROLLER 0xAA
#define TEST_PORT1 0xAB
#define DISABLE_SCANNING 0xF5




// data port responses //
#define CONTROLLER_TEST_SUCCESS 0x55
#define CONTROLLER_TEST_FAILURE 0xFC
#define PORT1_SUCCESS 0x00
#define PORT1_FAILURE_CLOCK 0x01

#define PORT_CLOCK_LINE_STUCK_LOW 0x01
#define PORT_CLOCK_LINE_STUCK_HIGH 0x02
#define PORT_DATA_LINE_STUCK_LOW 0x03
#define PORT_DATA_LINE_STUCK_HIGH 0x04

#define DEVICE_RESET_SUCCESS 0xFA
#define DEVICE_RESET_FAILURE 0xFC

#define ACK 0xFA


typedef struct ps2_device_t {
    uint16_t id;
    _Bool is_reset_success;
    // to be modified by irq handler. helps ps2 code know the response to interrupt data
    struct id_flow {
        _Bool is_scanning_disabled;
        _Bool is_identifying;
        _Bool has_received_id1;
        _Bool has_received_id2;
        _Bool is_complete;
    }id_flow;

}ps2_device_t;

ps2_device_t device;


uint8_t device_read_byte(void);
void device_write_byte(uint8_t data);



void ps2_init(void);
_Bool is_expecting_id_data(void);
void device_id_processor(uint8_t id_byte);
void set_device_id(uint8_t prepend_id_data);
void test_device_connectivity(void);

#endif

