#ifndef _TYPES_H
#define _TYPES_H
typedef unsigned int uint32_t;
typedef short int uint16_t;
typedef unsigned char uint8_t;
typedef struct registers
{
   uint32_t ds;                  // Data segment selector
   uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
   uint32_t int_no, err_code;    // Interrupt number and error code (if applicable)
   uint32_t eip, cs, eflags, useresp, ss; // Pushed by the processor automatically.
} registers_t;

#endif