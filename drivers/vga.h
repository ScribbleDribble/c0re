#include "../kernel/string.h"
#include "../kernel/kmalloc/kmalloc.h"
#include <stdbool.h>
#include <stdarg.h>
#include <stdatomic.h>

#ifndef _VGA_H
#define _VGA_H
#define VIDEO_ADDRESS 0x300b8000
#define MAX_HEIGHT 25
#define MAX_WIDTH 80

#define WHITE_ON_BLACK 0x0f
#define CHAR_SIZE 2


typedef struct position2D_t{
    unsigned int row;
    unsigned int col;
}position2D_t;

void klog(const char* s, ...);
void kputs(const char* str);
void kputc(const char character);
void clear_screen(void);


#endif 