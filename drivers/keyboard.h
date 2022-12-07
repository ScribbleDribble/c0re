#include "ps2.h"
#include "vga.h"

#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#define KEYBOARD_ID 0xAB83

// commands //
#define ENABLE_SCANNING 0xF4

void keyboard_init();

#endif