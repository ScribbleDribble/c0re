#include "../drivers/vga.h"
#include "surface.h"
#include "api/stdio.h"

// extern is_userspace;
// TODO stdio interface used to write to console
void userspace_test() { 
    clear_screen();
    kputs("hello");
    
    int i = 0;
    while (1) {
        i += 1;
    

    }

}

void userspace_test2() {
    int a = 0;
    char buf[32];
    kputs("no");
    while (1) {
        puts("Making a syscall!");
    }
}

void kernelspace_test() {
    klog("yo");
}
