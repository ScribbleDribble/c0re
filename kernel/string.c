#include "string.h"
#include "../drivers/serial_io.h"
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

void memory_set(void* src, unsigned int c, unsigned int n) {
	int i;
	char* ptr = (char*) src;
	for (i = 0; i < n; ++i) {
		ptr[i] = c;
	}
}

//todo: arbitrary blocks of memory copy (using void*)
void* memory_copy(void* dest, void* src, uint32_t n) {
	int i;

	char* to = (char*) dest;
	char* from = (char*) src;
	for (i = 0; i < n; ++i) {
		to[i] = from[i];
	}

	return to;
}

char* str_cpy(char* dest, const char *src) {
	int i = 0;
	while (src[i] != '\0') {
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return dest;
}

size_t str_len(const char* s) {
	size_t i = 0;
	while (s[i] != '\0' && i <= UINT_MAX-1) {
		i++;
	}
	return i;
}

int str_cmp(const char* s1, const char* s2) {
	size_t i = 0;
	while ((s1[i] != '\0' && s2[i] != '\0') && i <= UINT_MAX-1) {
		if (s1[i] != s2[i] && s1[i] > s2[i])
			return 1;

		else if (s1[i] != s2[i] && s1[i] < s2[i]) 
			return -1;
		i++;
	}
	if (s1[i] == s2[i]) {
		return 0;
	} 
	if (s1[i] != s2[i] && s1[i] > s2[i]){
		return 1;
	} else {
		return -1;
	}
}



void int_to_str(const int x, char* res, int len) {
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
	res[i] = '\0';

	return res;
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
