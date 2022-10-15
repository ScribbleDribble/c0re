#define VIDEO_ADDRESS 0xb8000
#define MAX_HEIGHT 24
#define MAX_WIDTH 80

#define WHITE_ON_BLACK 0x0f

// screen io ports
#define SCREEN_CTRL_PORT_ADDRESS
#define SCREEN_DATA_PORT_ADDRESS

void print_char(char character, unsigned char character_mode, int row, int col);
void kprint(char* string, unsigned char character_mode, int row, int col);
void clear_screen();
