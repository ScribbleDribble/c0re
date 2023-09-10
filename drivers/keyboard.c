#include "keyboard.h"


static StrMap* code_table;
KeyEvent* prev_key_event;

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
	// sm_put(code_table, "58", "CAPSLOCK");
    sm_put(code_table, "6C", "7");
	sm_put(code_table, "70", "o");
	sm_put(code_table, "74", "6");
	sm_put(code_table, "7C", "*");
	sm_put(code_table, "0D", "\t");
	sm_put(code_table, "15", "q");
	sm_put(code_table, "1D", "w");
	sm_put(code_table, "21", "c");
	sm_put(code_table, "25", "4");
	sm_put(code_table, "29", " ");
	sm_put(code_table, "2D", "r");
	sm_put(code_table, "31", "n");
	sm_put(code_table, "35", "y");
	sm_put(code_table, "3D", "7");
	sm_put(code_table, "41", ",");
	sm_put(code_table, "45", "0");
	sm_put(code_table, "49", ".");
	sm_put(code_table, "4D", "p");
	sm_put(code_table, "55", "=");
	sm_put(code_table, "0E", "`");
	sm_put(code_table, "16", "1");
	sm_put(code_table, "1A", "z");
	sm_put(code_table, "1E", "2");
	sm_put(code_table, "22", "x");
	sm_put(code_table, "26", "3");
	sm_put(code_table, "2A", "v");
	sm_put(code_table, "2E", "5");
	sm_put(code_table, "32", "b");
	sm_put(code_table, "36", "6");
	sm_put(code_table, "3A", "m");
	sm_put(code_table, "3E", "8");
	sm_put(code_table, "42", "k");
	sm_put(code_table, "46", "9");
	sm_put(code_table, "4A", "/");
	sm_put(code_table, "4E", "-");
	sm_put(code_table, "52", "'");
	sm_put(code_table, "5A", "ENTER");
	sm_put(code_table, "66", "BACKSPACE");
	sm_put(code_table, "1B", "s");
	sm_put(code_table, "23", "d");
	sm_put(code_table, "2B", "f");
	sm_put(code_table, "33", "h");
	sm_put(code_table, "3B", "j");
	sm_put(code_table, "43", "i");
	sm_put(code_table, "4B", "l");
	sm_put(code_table, "6B", "4");
	sm_put(code_table, "73", "5");

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
		register_interrupt_handler(IRQ1, &keypress_callback);
		prev_key_event = kmalloc(sizeof(KeyEvent));
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

void keypress_callback(void) {
	uint8_t data = device_read_byte();
	if (data == KEY_RELEASE_SCAN_CODE && prev_key_event->is_released != true) {
		prev_key_event->is_released = true;
		return;
	}
	char key[15];
	map_scan_code(data, key);
	if (prev_key_event->is_released) {
		prev_key_event->is_released = false;
		prev_key_event->key = '-';
		return;
	}
	kputc(key[0]);
	prev_key_event->key = key[0];
	prev_key_event->is_released = false;
	// pass to stdin buffer 
}

char* map_scan_code(uint8_t scan_code, char* key) {
	char scan_code_str[32];
	int_to_hex_str(scan_code, scan_code_str, 32);
	int is_scan_code_present = sm_exists(code_table, scan_code_str);
	if (is_scan_code_present == 0) {
		klog("Warning! Scan code %x is not mapped!", scan_code);
		return;
	}
	int result = sm_get(code_table, scan_code_str, key, sizeof(key));
}
