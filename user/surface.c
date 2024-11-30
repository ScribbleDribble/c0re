#include "surface.h"
#include "stdbool.h"


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

        
        if (*p == 1 && i % 500 == 0) {
            sleep(60);
        } 
        int_to_str(i, buf, 32);
        if (*p == 1 || *p == 2) {
            puts(buf);
        } 
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
