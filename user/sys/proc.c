#include "proc.h"

uint32_t fork() {
    // get current pid
    _fork();
}

uint8_t getpid() {
    _getpid();
}

void sleep(int seconds) {
    _sleep(seconds);
}