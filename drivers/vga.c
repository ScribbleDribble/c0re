#include "vga.h"

static unsigned int line;

void kprint_char(const char character, const unsigned char character_mode, position2D_t* pos) {

	if (!(pos->row < MAX_HEIGHT && pos->row >= 0 && pos->col < MAX_WIDTH && pos->col >= 0))
		return;
	
	char* mapped_addr = (char*) VIDEO_ADDRESS + CHAR_SIZE*(pos->col + pos->row*MAX_WIDTH); 
	*mapped_addr = character;
	mapped_addr++;
	*mapped_addr = character_mode;
	
}

static void kprint(const char* string, const unsigned char character_mode, position2D_t* pos) {
	
	if (pos->row >= MAX_HEIGHT) {
		scroll_down_one_line();
		pos->row -= 1;
		line = pos->row;
	}
	int i = 0;
	while(string[i] != '\0') {
		if (is_special_non_null_char(string[i])) {
			handle_special_non_null_char(string[i], pos);
		}
		
		if (string[i] == '\n') {
			pos->row++;
			pos->col = 0;
			i++;
			continue;
		}
		kprint_char(string[i], character_mode, pos);
		
		if (pos->col > MAX_WIDTH-1) {		
			pos->col = 0;
			pos->row++;	
		} else {
			pos->col++;
		}
		i++;
	}
}

void kputs(const char* str) {
	// maybe append \n to end, variable args -> string formatting e.g. kkputs("we have %i cookies", n_cookies)
	position2D_t pos = {line++, 0};
	kprint(str, 0x0f, &pos);
}

void putc(const char character) {
	position2D_t pos = {line++, 0};
	kprint_char(character, 0x0f, &pos);
}

void clear_screen() {
	char* addr = (char*) VIDEO_ADDRESS;
	memory_set(addr, 0, CHAR_SIZE*(MAX_HEIGHT*MAX_WIDTH + VIDEO_ADDRESS));
}

static _Bool is_special_non_null_char(const char c) {
	return c == '\n' || c =='\t';
}

static void scroll_down_one_line() {
	position2D_t local_pos = {0, 0};
	char* addr = (char*) VIDEO_ADDRESS + (MAX_WIDTH*2);
	for (; (unsigned int) addr < 2*(MAX_HEIGHT*MAX_WIDTH + VIDEO_ADDRESS) - MAX_WIDTH; addr += 2)
	{	
		if (local_pos.col == MAX_WIDTH) {
			local_pos.row++;
			local_pos.col = 0;
		}

		kprint_char(*addr, 0x0f, &local_pos);
		local_pos.col++;
	}
}

static void handle_special_non_null_char(char c, position2D_t* pos) {
	switch(c) {
		case '\n':
			if (line == MAX_HEIGHT-1){
				scroll_down_one_line();
			} else {
				pos->row += 1;
			}
			break;
	}
}