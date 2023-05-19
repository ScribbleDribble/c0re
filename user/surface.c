#include "../drivers/vga.h"
#include "surface.h"
extern switch_to_kernelspace(void);
// extern is_userspace;
// TODO stdio interface used to write to console
void userspace_test() { 

    // char buf[32];
    clear_screen();
    kputs("hello");
    
    int i = 0;
    while (1) {
        i += 1;
        
        // if (i % 66 == 0) {
        //     int_to_str(1, buf, 10);
        //     kputs(buf);
        // }

    }
    // int a = 1/0;
    // switch_to_kernelspace();
    
    // calling a privileged instruction yields a GPF 
    // asm volatile ("int $0x3");

}

void userspace_test2() {
    int a = 0;
    char buf[32];
    kputs("no");
    int x, y, z = 100;
    int i = 0;
    while (1) {
        puts("a");
    }
}

void kernelspace_test() {
    klog("yo");
}
