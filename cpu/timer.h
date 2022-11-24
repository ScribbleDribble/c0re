#include "../kernel/string.h"
#include "types.h"
#include "irq.h"
#include "port_io.h"

#define INPUT_CLOCK 1193180 //in hz
#define PIT_CMD_PORT 0x43
#define PIT_DATA_PORT 0x40
#define REPEAT_CMD 0x36

void init_timer(unsigned int frequency);
