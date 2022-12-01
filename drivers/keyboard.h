#include "ps2.h"
#include "vga.h"

#define KEYBOARD_ID 0xAB83

// commands //
#define ENABLE_SCANNING 0xFA

void keyboard_init();