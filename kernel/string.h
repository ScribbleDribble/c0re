#ifndef _STR_H // _STRING_H breaks build of "test_strings.cpp"!!
#define _STR_H

#include <stddef.h>
#include <limits.h> 

void* memory_copy(void* src, void* dest, int n);
void memory_set(void* src, unsigned int c, unsigned int n);
size_t str_len(const char* s);
char* str_cpy(char* dest, const char* src);
int str_cmp(const char* s1, const char* s2);
void reverse(char* str, int len);
void int_to_str(int x, char* res, int len);
void int_to_hex_str(int x, char* res, int len);

#endif 