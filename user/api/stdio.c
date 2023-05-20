#include "stdio.h"


void puts(const char* s) {
    if (*s == '2')
    {
        asm("xchg %bx, %bx");
    }
    _puts(s);
}