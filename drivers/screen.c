#include "screen.h"


static unsigned int line;

void kprint_char(const char character, const unsigned char character_mode, const int row, const int col) {

	if (!(row < MAX_HEIGHT && row >= 0 && col < MAX_WIDTH && col >= 0))
		return;
	
	char* mapped_addr = (char*) VIDEO_ADDRESS + CHAR_SIZE*(col + row*MAX_WIDTH); 
	*mapped_addr = character;
	mapped_addr++;
	*mapped_addr = character_mode;
	
}

static void kprint(const char* string, const unsigned char character_mode, int row, int col) {
	
	if (row >= MAX_HEIGHT) {
		scroll_down_one_line();
		row -= 1;
		line = row;
	}
	int i = 0;
	while(string[i] != '\0') {

		if (is_special_non_null_char(string[i])) {
			position2D_t new_position = handle_special_non_null_char(string[i], row, col);
			row = new_position.row;
			col = new_position.col;
		}
		
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
	for (; (unsigned int) addr < 2*(MAX_HEIGHT*MAX_WIDTH + VIDEO_ADDRESS); addr += 2)
	{
		*addr = ' ';
	}
}

static _Bool is_special_non_null_char(const char c) {
	return c == '\n' || c =='\t';
}

static void scroll_down_one_line() {
	int local_col = 0;
	char* addr = (char*) VIDEO_ADDRESS + (MAX_WIDTH*2);
	int local_row = 0;
	for (; (unsigned int) addr < 2*(MAX_HEIGHT*MAX_WIDTH + VIDEO_ADDRESS) - MAX_WIDTH; addr += 2)
	{	
		if (local_col == MAX_WIDTH) {
			local_row++;
			local_col = 0;
		}
		kprint_char(*addr, 0x0f, local_row, local_col++);
	}
}


static position2D_t handle_special_non_null_char(char c, int row, int col) {
	position2D_t pos = {pos.row = row, pos.col = col}; 
	switch(c) {
		case '\n':
			if (line == MAX_HEIGHT-1){
				scroll_down_one_line();
			} else {
				pos.row += 1;
			}
			break;
	}
}