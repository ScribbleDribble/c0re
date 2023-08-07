
#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#include <stdbool.h>
#include "process.h"
#include "../user/surface.h"


pcb_t* schedule(const registers_t* context, interrupt_state_t* int_state);

#endif 