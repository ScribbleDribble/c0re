#ifndef _TIMER_H
#define _TIMER_H

#include "../kernel/string.h"
#include "irq.h"
#include "port_io.h"
#include <stdint.h>
#include "../common/types.h"
#include "../drivers/vga.h"
#include "../kernel/scheduler.h"
#include "../cpu/multitasking/tss.h"

#define INPUT_CLOCK 1193180 //in hz
#define PIT_CMD_PORT 0x43
#define PIT_DATA_PORT 0x40
#define REPEAT_CMD 0x36
#define USER_DATA_SEGMENT 0x23

int configure_timer(unsigned int frequency);
#endif