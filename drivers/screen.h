#define VIDEO_ADDRESS 0xb8000
#define MAX_HEIGHT 24
#define MAX_WIDTH 80

#define WHITE_ON_BLACK 0x0f
#define CHAR_SIZE 2

typedef struct position2D_t{
    unsigned int row;
    unsigned int col;
}position2D_t;

void puts(const char* str);
void putc(const char character);
void clear_screen();
static position2D_t handle_special_non_null_char(char c, int row, int col);
static void scroll_down_one_line();
static _Bool is_special_non_null_char(const char c);