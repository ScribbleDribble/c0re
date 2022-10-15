#include "screen.h"


void kprint_char(char character, unsigned char character_mode, int row, int col) {

	if (!(row < MAX_HEIGHT && row >= 0 && col < MAX_WIDTH && col >= 0))
		return;
	
	char* mapped_addr = (char*) VIDEO_ADDRESS + 2*(col + row*MAX_WIDTH); 
	*mapped_addr = character;
	mapped_addr++;
	*mapped_addr = character_mode;
	
}

void kprint(char* string, unsigned char character_mode, int row, int col) {
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

void clear_screen() {
	int col;
	int i = 0;
	char* addr = (char*) VIDEO_ADDRESS;
	for (; addr < MAX_HEIGHT*MAX_WIDTH + VIDEO_ADDRESS; addr += 2)
	{
		*addr = ' ';
	}
}


