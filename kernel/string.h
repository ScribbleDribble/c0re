#ifndef _STR_H // _STRING_H breaks build of "test_strings.cpp"!!
#define _STR_H

void memory_copy(void* src, void* dest, int n);
void memory_set(void* src, unsigned int c, unsigned int n);
char* strcpy(char* dest, const char* src);
void reverse(char* str, int len);
void int_to_str(int x, char* res, int len);
void int_to_hex_str(int x, char* res, int len);

#endif 