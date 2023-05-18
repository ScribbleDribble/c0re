
#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#include <stdbool.h>
#include "process.h"


pcb_t* schedule(registers_t* context);

#endif 