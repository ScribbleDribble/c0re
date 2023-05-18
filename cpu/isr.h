#include <stdint.h>

#define ISR0 0 // div by 0
#define ISR1 1  // debug
#define ISR2 2  // non maskable interrupt
#define ISR3 3  // breakpoint
#define ISR4 4  // overflow
#define ISR5 5  // bound range exceeded
#define ISR6 6  // invalid opcode
#define ISR7 7  // device not available
#define ISR8 8  // double fault
#define ISR9 9  // coprocessor segment overrun
#define ISR10 10    // invalid TSS
#define ISR11 11    // segment not present
#define ISR12 12    // stack segment fault
#define ISR13 13    // GPF
#define ISR14 14    // page fault
#define ISR15 15    // reserved
#define ISR16 16    // x87 floating-point exception
#define ISR17 17    // alignment check
#define ISR18 18    // machine check    

extern void _isr0(void);
extern void _isr1(void);
extern void _isr2(void);
extern void _isr3(void);
extern void _isr4(void);
extern void _isr5(void);
extern void _isr6(void);
extern void _isr7(void);
extern void _isr8(void);
extern void _isr9(void);
extern void _isr10(void);
extern void _isr11(void);
extern void _isr12(void);
extern void _isr13(void);
extern void _isr14(void);
extern void _isr15(void);
extern void _isr16(void);
extern void _isr17(void);
extern void _isr18(void);




