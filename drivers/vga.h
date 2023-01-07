#include "../kernel/string.h"
#include <stdbool.h>

#ifndef _VGA_H
#define _VGA_H
#define VIDEO_ADDRESS 0xb8000
#define MAX_HEIGHT 24
#define MAX_WIDTH 80

#define WHITE_ON_BLACK 0x0f
#define CHAR_SIZE 2


typedef struct position2D_t{
    unsigned int row;
    unsigned int col;
}position2D_t;

void kputs(const char* str);
void kputc(const char character);
void clear_screen(void);


#endif 