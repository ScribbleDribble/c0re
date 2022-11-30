#include "keyboard.h"


void keyboard_init() {
    if (device.id == KEYBOARD_ID)
    {
        puts("Keyboard detected!");
    }
    // enable scanning
    device_write_byte(0xF4);
}

