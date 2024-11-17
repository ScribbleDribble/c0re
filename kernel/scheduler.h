
#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#include <stdbool.h>
#include "process.h"
#include "../util/priority_queue.h"
#include "../user/surface.h"


pcb_t* schedule(const registers_t* context, interrupt_state_t* int_state);
uint32_t schedule_new_fork(const registers_t context, interrupt_state_t int_state);
uint32_t get_running_proc_pid(void);

#endif 