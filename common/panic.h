

void panic(const char* msg) {
    kputs(msg);
    while (1);
}