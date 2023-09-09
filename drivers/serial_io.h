#include "../cpu/timer.h"
#include <stdarg.h>


#define COM1_PORT 0x3F8
#define UART_CLOCK 115200
#define LINE_CONTROL_REG_OFFSET 3
#define DLAB_ON_SET_LOW_DIVISOR_OFFSET 0
#define DLAB_ON_SET_HIGH_DIVISOR_OFFSET 1

#define CHAR_LENGTH 7
#define CHAR_LENGTH_BITS_SETTING 2 

#define INTERRUPT_ENABLE_REG_OFFSET 1


void serial_init(void);