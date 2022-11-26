#include "screen.h"


static unsigned int line;

void kprint_char(const char character, const unsigned char character_mode, const int row, const int col) {

	if (!(row < MAX_HEIGHT && row >= 0 && col < MAX_WIDTH && col >= 0))
		return;
	
	char* mapped_addr = (char*) VIDEO_ADDRESS + 2*(col + row*MAX_WIDTH); 
	*mapped_addr = character;
	mapped_addr++;
	*mapped_addr = character_mode;
	
}

static void kprint(const char* string, const unsigned char character_mode, int row, int col) {
	if (!(row < MAX_HEIGHT && row >= 0 && col < MAX_WIDTH && col >= 0))
		return;

	int i = 0;
	while(string[i] != '\0') {
		
		if (string[i] == '\n') {
			row++;
			col = 0;
			i++;
			continue;
		}
		kprint_char(string[i], character_mode, row, col);
		
		if (col > MAX_WIDTH-1) {		
			col = 0;
			row++;	
		} else {
			col++;
		}
		i++;
	}

}

void puts(const char* str) {
	// TODO consider implement scrolling for when line >= HEIGHT (just copy everything but top line and include new str). also append \n to end
	// variable args 
	// string formatting e.g. kputs("we have %i cookies", n_cookies)
	kprint(str, 0x0f, line++, 0);
}

void putc(const char* character) {
	kprint_char(character, 0x0f, line++, 0);
}

void clear_screen() {
	int col;
	int i = 0;
	char* addr = (char*) VIDEO_ADDRESS;
	for (; addr < MAX_HEIGHT*MAX_WIDTH + VIDEO_ADDRESS; addr += 2)
	{
		*addr = ' ';
	}
}


