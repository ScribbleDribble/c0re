#include "../drivers/screen.h"
#include "string.h"
#include "../cpu/idt.h"

int main() {
	int row = 0;
	int col = 0;
	
	clear_screen();
	// char str_15[5];
	// char str_hex_16[5];
	// char str_2001[8];
	// char str_10[6];
	// char str_hex_555[8];
	// int_to_str(10, str_10, 6);
	// int_to_str(2001, str_2001, 8);
	// int_to_hex_str(16, str_hex_16, 5);
	// int_to_hex_str(555, str_hex_555, 8);
	// kprint(str_10, 0x0f, 2, 2);
	// kprint(str_hex_16, 0x0f, 3, 3);
	// kprint(str_2001, 0x0f, 4, 4);
	// kprint(str_hex_555, 0x0f, 5, 5);
	kprint("Micah's OS", 0x0f, 0, 0);

	init_idt();

	// generates cpu exception, triggering isr0
	int val = 5/0;

}
		
