#ifndef _PORT_IO_H
#define _PORT_IO_H


unsigned char port_byte_read(unsigned short port);
void port_byte_write(unsigned short port, unsigned char data);
unsigned short port_word_read(unsigned short port);
void port_word_write(unsigned short port, unsigned short data);

#endif