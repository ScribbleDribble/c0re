#include "driver_entry.h"


void init_drivers() {
    configure_timer(10);
    ps2_init();
	keyboard_init();
}