#ifndef _TYPES_H
#define _TYPES_H

#include <stdint.h>

// typedef struct registers_t {
//     int AX, CX, DX, BX, SP, BP, SI, DI;
// }registers_t __attribute__((packed));

typedef struct registers_t {
    int DI, SI, BP, SP, BX, DX, CX, AX;
}__attribute__((packed)) registers_t ;


#endif 