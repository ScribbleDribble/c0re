#include "keyboard.h"


static StrMap* code_table;

void create_scan_table() {
    
	char buf[255];
	int result;

	code_table = sm_new(SCAN_CODES_TOTAL);

	if (code_table == NULL) {
		/* Handle allocation failure... */
		klog("Allocation failure...");
	}
    
	sm_put(code_table, "1C", "a");
	sm_put(code_table, "24", "e");
	sm_put(code_table, "2C", "t");
	sm_put(code_table, "34", "g");
	sm_put(code_table, "3C", "u");
	sm_put(code_table, "44", "o");
	sm_put(code_table, "4C", ";");
	sm_put(code_table, "54", "[");
	sm_put(code_table, "58", "CAPSLOCK");
    sm_put(code_table, "6C", "7");
	sm_put(code_table, "70", "O");
	sm_put(code_table, "74", "6");
	sm_put(code_table, "78", "F11");
	sm_put(code_table, "58", "caps-lock");
    // implement the rest 

	result = sm_get(code_table, "1C", buf, sizeof(buf));

	if (result == 0) {
		klog("Failed to create scan table");
	} else {
        klog("Created scan table");
    }

}

void keyboard_init() {
    if (device.id == KEYBOARD_ID) {
        kputs("Keyboard detected!");
    } else {
        kputs("Keyboard not detected on PS/2 port 1.");
		return;
    }
    // enable scanning
	device_write_byte(ENABLE_SCANNING);

	klog(">[Keyboard] Scanning enabled");
    create_scan_table();
    // get scan code set (either 1, 2 or 3)
}

