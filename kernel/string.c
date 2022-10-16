

void reverse(char* str, int arr_len) {
	int i = 0;
	// don't forget null terminator
	int j = arr_len - 2;
	char temp;
	while (i >= j) {
		temp = str[i];
		str[i++] = str[j];
		str[j--] = temp;
	}
}


void int_to_str(int x, char* res) {
	// we do not have dynamic memory allocation yet. so use fixed sized array for now
	int i = 0;
	
	while (x >= 1) {
		char ascii_char = '0';
		int rem = x % 10;
		ascii_char += rem;
		res[i++] = ascii_char;
		
		x /= 10;
	}
	res[4] = '\0';
	reverse(res, 5);
}
