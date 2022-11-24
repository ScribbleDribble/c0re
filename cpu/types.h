#ifndef _TYPES_H
#define _TYPES_H
typedef unsigned int uint32_t;
typedef short int uint16_t;
typedef unsigned char uint8_t;
typedef struct registers_t {
    uint16_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t interrupt_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
}registers_t;


#endif