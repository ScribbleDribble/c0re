#include "panic.h"

void panic(const char* msg) {
    kputs(msg);
    while (1);
}

void assert_true(bool result, const char* failure_msg) {
    if (!result) {
        panic(failure_msg);
    }
}
