#include <stdint.h>

extern uint32_t _fork(void);
extern void _sleep(int);

uint32_t fork(void);
uint8_t getpid(void);
void sleep(int);

