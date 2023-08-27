#ifndef _PANIC_H
#define _PANIC_H

#include <stdbool.h>
#include "../drivers/vga.h"

void panic(const char* msg);
void assert_true(bool result, const char* failure_msg);

#endif