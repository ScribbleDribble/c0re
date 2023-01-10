#include "string.h"

void reverse(char* str, int len) {
	int i = 0;
	// leave \0 in place
	int j = len - 1;
	char temp;
	while (i < j) {
		temp = str[i];
		str[i++] = str[j];
		str[j--] = temp;
	}
}

void memory_set(void* src, unsigned int c, int n) {
	int i;
	char* ptr = (char*) src;
	for (i = 0; i < n; ++i) {
		ptr[i] = c;
	}
}

//todo: arbitrary blocks of memory copy (using void*)
void memory_copy(void* dest, void* src, int n) {
	int i;

	char* to = (char*) dest;
	char* from = (char*) src;
	for (i = 0; i < n; ++i) {
		to[i] = from[i];
	}
}

void int_to_str(const int x, char* res, int len) {
	// we do not have dynamic memory allocation yet. so use fixed sized array for now
	int i = 0;
	int tmp = x;

	if (x == 0) {
		res[i++] = '0';
	} else if (x < 0) {
		tmp *= -1;
	}
	
	while (tmp >= 1) {
		char ascii_char = '0';
		int rem = tmp % 10;
		ascii_char += rem;
		res[i++] = ascii_char;
		tmp /= 10;
	}

	if (x < 0)
		res[i++] = '-';

	reverse(res, i);
	res[len-1] = '\0';

}

void int_to_hex_str(int x, char* res, int len) {
	int i = 0;
	if (x == 0) {
		res[i++] = '0';
	}
	while (x >= 1) {
		int rem = x % 16;
		int ascii_char = '0';
		if (rem > 9) {
			ascii_char = 'A';
			rem -= 10;
		}
		ascii_char += rem;
		res[i++] = ascii_char;
		x /= 16;
	}

	res[i] = '\0';
	reverse(res, i);
}

