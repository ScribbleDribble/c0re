#include "../drivers/vga.h"
#include "stdio.h"
#include "../kernel/scheduler.h"
#include "surface.h"

extern current_pid;
// extern is_userspace;
// TODO stdio interface used to write to console
void userspace_test() { 
    clear_screen();
    char buf[32];
    int i = 5000;
    while (1) {
        i++;
        int_to_str(i, buf, 32);
        puts(buf);
    }

}

void userspace_test2() {
    int a = 0;
    char buf[32];
    int i = 0;
    while (1) {
        i++;
        int_to_str(current_pid, buf, 32);
        puts(buf);
    }
}
