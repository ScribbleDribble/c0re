#include "ps2.h"
#include "vga.h"
#include "../util/strmap.h"

#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#define KEYBOARD_ID 0xAB83
#define SCAN_CODES_TOTAL 0xD9

#define KEY_RELEASE_SCAN_CODE 0xF0

typedef struct KeyEvent {
    char key;
    bool is_released;
}KeyEvent;

// commands //
#define ENABLE_SCANNING 0xF4

void keyboard_init(void);
void keypress_callback(void);
char* map_scan_code(uint8_t scan_code, char* key);
#endif