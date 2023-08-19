#include "surface.h"

void userspace_test() { 
    clear_screen();
    char buf[32];
    int i = 0;
    while (1) {
    }

}

void userspace_test2() {
    int a = 0;
    char buf[32];
    int i = 0;
    while (1) {
        i++;
        int_to_str(i, buf, 32);
        // puts(buf);
    }
}
