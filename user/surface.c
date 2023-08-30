#include "surface.h"
#include <stdint.h>

extern uint8_t current_pid;
extern uint16_t n_procs;
void userspace_test() { 
    clear_screen();
    char buf[32];
    int i = 0;
    int j = 1;
    int* p = (int*)0x22000;
    while (1) {
        // this value will be set only by process 0 and it cannot be read by other processes.
        if (current_pid == 0 && n_procs > 1) {
            *p = 0x192;
        } 
        int_to_str(*p, buf, 32);
        puts(buf);

        i++;
    }

}

void userspace_test2() {
    char buf[32];
    int i = 0;
    while (1) {
        i++;
        int_to_str(i, buf, 32);
        // puts(buf);
    }
}
