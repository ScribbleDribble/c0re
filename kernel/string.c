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


void memset(int* src, int c, int n) {
	int i;
	for (i = 0; i < n; ++i) {
		*src = c;
		src++;
	}
}

void int_to_str(int x, char* res, int len) {
	// we do not have dynamic memory allocation yet. so use fixed sized array for now
	int i = 0;

	if (x == 0)
		res[i++] = '0';
	
	while (x >= 1) {
		char ascii_char = '0';
		int rem = x % 10;
		ascii_char += rem;
		res[i++] = ascii_char;
		x /= 10;
	}

	res[i] = '\0';
	reverse(res, i);
}

void int_to_hex_str(int x, char* res, int len) {

	int i = 0;
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

