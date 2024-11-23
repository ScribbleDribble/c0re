#include "surface.h"
#include "stdbool.h"

extern uint8_t current_pid;
extern uint16_t n_procs;
void userspace_test() { 
    clear_screen();
    char buf[32];
    int i = 0;
    int j = 1;
    int* p = (int*)0x22000;
    int pid = fork();
    pid = fork();
    *p = getpid();
    bool hasSlept = false;
    while (1) {

        if (!hasSlept) {
            // sleep(5);
            hasSlept = true;
        }
        // this value cannot be read by other processes
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
