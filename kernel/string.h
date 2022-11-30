#ifndef _STR_H // _STRING_H breaks build of "test_strings.cpp"!!
#define _STR_H

void memory_copy(char* src, char* dest, int n);
void memory_set(char* src, unsigned int c, int n);
void reverse(char* str, int len);
void int_to_str(int x, char* res, int len);
void int_to_hex_str(int x, char* res, int len);

#endif 