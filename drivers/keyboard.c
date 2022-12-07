#include "keyboard.h"


// ideally have a hash table that maps a series of 

void keyboard_init() {
    if (device.id == KEYBOARD_ID) {
        kputs("Keyboard detected!");
    } else {
        kputs("Keyboard not detected on PS/2 port 1.");
    }

    // enable scanning
    device_write_byte(ENABLE_SCANNING);

    // get scan code set (either 1, 2 or 3)
}

