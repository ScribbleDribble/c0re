
unsigned char port_byte_read(unsigned short port) {
	unsigned char data;
	// "=a" (result) places value stored in al into the variable data. 
	// "d" (port) places port variable into the dx register
	__asm__("in %%dx %al" : "=a" (data) : "d" (port)); // note that we are loading data for this asm instruction on the fly. 
	return data;
}


void port_byte_write(unsigned short port, unsigned char data) {
	__asm__("out %%al %%dx", "a" (data), "d" (port));
}

unsigned short port_word_read(unsigned short port) {
	unsigned char data;
	__asm__("in %%dx %%al" : "=a" (data) "d" (port));
	return data;
}

void port_word_write(unsigned short port, unsigned short data) {
	__asm__("out %%al %%dx" : "a" (data) "d" (port));
}

