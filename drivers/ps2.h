#ifndef _PS2_H
#define _PS2_H

#include "../cpu/port_io.h"
#include "../kernel/string.h"
#include <stdint.h>

#define CMD_PORT 0x64
#define DATA_PORT 0x60 // used for reading data that was received from a PS/2 device or from the PS/2 controller itself and writing data to a PS/2 device or to the PS/2 controller itself.
#define STATUS_PORT 0x64
#define CONFIG_REGISTER_PORT 0x20

// commands // 
#define DISABLE_PORT1 0xAD
#define DISABLE_PORT2 0xA7







#endif