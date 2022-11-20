#include "../drivers/screen.h"
#include "string.h"
#include "../cpu/idt.h"

int main() {
	int row = 0;
	int col = 0;
	
	clear_screen();
	puts("Micah's OS");
	puts("Testing this out");
	init_idt();

	// generates cpu exception, triggering isr0

	// generates a hardware interrupt
	__asm__ __volatile__ ("sti");
	// int val = 5/0;

}
		
