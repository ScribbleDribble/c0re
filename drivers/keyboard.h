#include "ps2.h"
#include "vga.h"
#include "../util/strmap.h"

#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#define KEYBOARD_ID 0xAB83
#define SCAN_CODES_TOTAL 0xD9

// commands //
#define ENABLE_SCANNING 0xF4

void keyboard_init(void);

#endif