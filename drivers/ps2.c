#include "ps2.h"

// PS/2 Module - to be utilised by all PS/2 devices // 
// implementation of https://wiki.osdev.org/%228042%22_PS/2_Controller#Translation

ps2_device_t device = {0};

static void poll_write_buf_ready_status(void);
static void identify_devices(void);

// irq handler for ps2 devices. will call device driver specific code. 
void ps2_device_callback() {
    uint8_t data = device_read_byte();
    char buf[16];
    int_to_hex_str(data, buf, 16);
    kputs(buf);
    
    switch(data) {
        case ACK:
            if (!device.is_reset_success) {
                device.is_reset_success = 1;
                break;
            }
            if (!device.id_flow.is_scanning_disabled){
                device.id_flow.is_scanning_disabled = 1;
                break;
            }
            else if (!device.id_flow.is_identifying){
                device.id_flow.is_identifying = 1;
                break;
            }
            break;
        case DEVICE_RESET_FAILURE:
            device.is_reset_success = 0;
            break;
        default:
            if (is_expecting_id_data()) {
                device_id_processor(data);
                return;
            }
            // kputs(">PS/2 Unhandled response to device!");
    }
}


void device_id_processor(uint8_t id_byte) {
    if(device.id == 0) {
        set_device_id(id_byte);
        device.id_flow.has_received_id1 = 1;
        return;
    }
    device.id_flow.has_received_id2 = 1;
    set_device_id(id_byte);
    return;
}

// we need to prepend the new id data to the current id.
void set_device_id(uint8_t prepend_id_data) {

    if (device.id == 0) {
        device.id = prepend_id_data;
    } else {
        device.id = (device.id << 8) | prepend_id_data;
        device.id_flow.is_complete = 1;
    }
}


/**
    inits ps/2 controller by resetting and clearing its data 
    pauses irqs and writes from the device for non-disturbance.
    also tests to see if CONTROLLER functions as expected. 
 */
void ps2_init() {
    // todo - break up into functions

    // disable devices so that incoming data overwrite data we request
    port_byte_write(CMD_PORT, DISABLE_PORT1);
    port_byte_write(CMD_PORT, DISABLE_PORT2);

    // flush buffer by performing read
    port_byte_read(DATA_PORT);
    uint8_t status_flag = port_byte_read(STATUS_PORT);

    // if first bit of status flag is set, then there is data to be consumed. 
    // keep on performing reads until buffer is flushed 
    while ((status_flag & 1)) {
        kputs(">Flushing ps/2 output port");
        port_byte_read(DATA_PORT);
    }

    port_byte_write(CMD_PORT, READ_CONTROLLER_CONFIG);
    uint8_t controller_config_flag = port_byte_read(DATA_PORT);
    // this flag configures our ps/2 devices.
    // 1. we want to disable IRQs from both ps/2 devices (ports) so we are not interrupted during setup
    // 2. disbale translation (not sure why but its suggested)
    // so we clear bits 0,1 and 6 and write that back into the controller.
    uint8_t setup_mask = 0b00111101;
    uint8_t new_controller_config_flag = controller_config_flag & setup_mask;
    port_byte_write(CMD_PORT, WRITE_CONTROLLER_CONFIG);
    port_byte_write(DATA_PORT, new_controller_config_flag);

    
    port_byte_write(CMD_PORT, TEST_CONTROLLER);
    status_flag = port_byte_read(STATUS_PORT);
    while ((status_flag & 0));

    uint8_t controller_response = port_byte_read(DATA_PORT);
    switch (controller_response) {
        case CONTROLLER_TEST_SUCCESS:
            kputs(">PS/2 Test successful!");
            break;
        case CONTROLLER_TEST_FAILURE:
            kputs(">PS/2 Controller Test failed!");
            break;
        default:
            kputs(">PS/2 Unexpected response from controller test!");
    }

    // seems like qemu doesnt support dual channels so just test the first port for now.
    port_byte_write(CMD_PORT, TEST_PORT1);
    status_flag = port_byte_read(STATUS_PORT);
    while (status_flag & 0);
    controller_response = port_byte_read(DATA_PORT);
    switch(controller_response) {
        case PORT1_SUCCESS:
            kputs(">PS/2 Port 1 success!");
            break;
        case PORT_CLOCK_LINE_STUCK_LOW:
            kputs(">PS/2 port failure: Data line stuck low");
            break;
        case PORT_CLOCK_LINE_STUCK_HIGH:
            kputs(">PS/2 port failure: Clock line stuck high");
            break;
        case PORT_DATA_LINE_STUCK_LOW:
            kputs(">PS/2 port failure: Data line stuck low");
            break;
        case PORT_DATA_LINE_STUCK_HIGH:
            kputs(">PS/2 port failure: Data line stuck high");
            break;
        default:
            kputs(">PS/2 Unhandled response to port test failure!");
    }

    // enable devices
    port_byte_write(CMD_PORT, ENABLE_PORT1);
    port_byte_write(CMD_PORT, READ_CONTROLLER_CONFIG);
    while (status_flag & 0);
    controller_config_flag = port_byte_read(DATA_PORT);
    port_byte_write(CMD_PORT, WRITE_CONTROLLER_CONFIG);
    port_byte_write(DATA_PORT, controller_config_flag | 10000000); 

    register_interrupt_handler(IRQ1, &ps2_device_callback);

    test_device_connectivity();

    // add device irq handler 
    identify_devices();
}

void test_device_connectivity() {
    // test device connected to port 1
    // TODO generalise for both ports;
    char str[32];
    memory_set(str, 0, 32);

    device_write_byte(RESET_DEVICE);
    while (!device.is_reset_success) {
        kputs(">PS/2 Waiting for device reset to be successful");
    }
    kputs(">PS/2 Device Reset successful!");
}

// data is ready to be written
static void poll_write_buf_ready_status() {
    const int is_busy = 1;
    uint8_t status_flag = port_byte_read(STATUS_PORT);
    while ((status_flag >> 1) & (uint8_t) is_busy) {
        status_flag = port_byte_read(STATUS_PORT);
        kputs(">PS/2 interface driver: Polling write buffer..");
    }
}

// ONLY IDENTIFIES DEVICE ON PORT 1 atm!
static void identify_devices() {
    // would be better to have timeout logic incase this isnt successful

    device_write_byte(DISABLE_SCANNING);
    while (!device.id_flow.is_scanning_disabled)
        kputs(">PS/2 Waiting for scanning to be disabled");

    kputs(">PS/2 Received ACK for disabling scanning");

    device_write_byte(IDENTIFY_DEVICE);
    while (!device.id_flow.is_identifying)
        kputs(">PS/2 Waiting for Ack for device identification");
    kputs(">PS/2 Received ack for identifying device");
    char buf[17];

    while(!device.id_flow.has_received_id1)
        kputs(">PS/2 Waiting for id part1");
    while(!device.id_flow.has_received_id2)
        kputs(">PS/2 Waiting for id part2");

    device.id_flow.is_complete = 1;
    int_to_hex_str(device.id, buf, 17);

    kputs(">PS/2 Device identification complete - device id is: ");
    kputs(buf);
    
}

_Bool is_expecting_id_data() {
    return device.id_flow.is_identifying && !device.id_flow.is_complete;
}

// writes to the device - intended to be for the ps/2 device drivers (keyboard, mouse etc)
// ONLY FOR ACCESSING PORT 1 - TODO consider both ports 
void device_write_byte(uint8_t data) {
    poll_write_buf_ready_status();
    port_byte_write(DATA_PORT, data);
}
// ONLY FOR ACCESSING PORT 1 - TODO consider both ports 
uint8_t device_read_byte() {
    return port_byte_read(DATA_PORT);
}
