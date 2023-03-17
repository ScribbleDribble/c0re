#include "vga.h"

static unsigned int line = 0;

static void kprint_char(const char character, const unsigned char character_mode, position2D_t* pos) {

	if (!(pos->row < MAX_HEIGHT && pos->col < MAX_WIDTH))
		return;
	
	char* mapped_addr = (char*) (VIDEO_ADDRESS + CHAR_SIZE*(pos->col + pos->row*MAX_WIDTH)); 
	*mapped_addr = character;
	mapped_addr += 1;
	*mapped_addr = character_mode;
	
}

static bool is_special_non_null_char(const char c) {
	return c == '\n' || c =='\t';
}

void scroll_down_one_line(void) {
	memory_copy(
				(char*) VIDEO_ADDRESS, 
				(char*) VIDEO_ADDRESS + (MAX_WIDTH*CHAR_SIZE), 
				CHAR_SIZE*(MAX_HEIGHT*MAX_WIDTH) - CHAR_SIZE*MAX_WIDTH
				);
	memory_set(VIDEO_ADDRESS + CHAR_SIZE*((MAX_HEIGHT-1)*MAX_WIDTH), 0, MAX_WIDTH*CHAR_SIZE); 
}

static void handle_special_non_null_char(char c, position2D_t* pos) {
	switch(c) {
		case '\n':
			if (line == MAX_HEIGHT-1){
				scroll_down_one_line();
			} else {
				pos->row += 1;
				pos->col = 0;
			}
			break;
	}
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

char* format_type(const char specifier, va_list* arg_list, char* buf) {
	switch(specifier) {
		case 'i':
			int_to_str(va_arg(*arg_list, int), buf, 32);
			return buf;
		case 'x':
			int_to_hex_str(va_arg(*arg_list, int), buf, 32);
			return buf;
		case 's':
			return va_arg(*arg_list, char*);

		default:
			return "INVALID_SPECIFIER";
	}
}

// like printf but writes with a newline 
void klog(const char* s, ...) {
	// seems like this variable isnt used...

	int arg_count = 100;
	va_list arg_list;
	va_start(arg_list, arg_count);
	char tmp[32];
	char* res = kmalloc(1);
	int i;
	int j = 0;
	for (i = 0; s[i] != '\0'; i++) {
		if (s[i] == '%') {
			char* str = format_type(s[i+1], &arg_list, tmp);
			int new_size = str_len(str) + j;
			res = realloc(res, new_size + 2);
			str_cpy(res + j, str);
			j = new_size;
			i += 1;
		} else {
			res[j] = s[i];
			res = realloc(res, j+2);
			j += 1;
		}
	}
	res[j] = '\0';
	
	kputs(res);
	free(res);
	va_end(arg_list);
}


void kputs(const char* str) {
	line += 1;
	position2D_t pos = {line, 0};
	kprint(str, 0x0f, &pos);
}

void kputc(const char character) {
	line += 1;
	position2D_t pos = {line, 0};
	kprint_char(character, 0x0f, &pos);
}

void clear_screen() {
	char* addr = (char*) VIDEO_ADDRESS;
	memory_set(addr, 0, CHAR_SIZE*(MAX_HEIGHT*MAX_WIDTH));
	line = 0;
}