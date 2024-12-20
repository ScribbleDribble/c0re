

# Core
A simple 32-bit monolithic kernel for x86 which includes:
- Virtual and physical memory management
- Software multitasking
- System calls (fork, getpid, etc)
- Higher-half kernel memory
- Heap memory allocator
- Stdio


## Dependencies

- nasm
- qemu or bochs 
    - to use the bochs gui debugger, you must compile from the source. Configure bochs to enable the debugger and use the x display manager.
    ```
    ./configure --with-x --with-x11 --enable-debugger --enable-debugger-gui
    make
    make install
    ```
    
- i386 cross-compiler (visit here for a tutorial https://github.com/cfenollosa/os-tutorial/tree/master/11-kernel-crosscompiler)
    - if using gcc-11 or higher, you must build the crosscompiler against binutils version >= 2.35.2 
- xorriso

## Setup

To build the kernel
```
make
```

You can now choose to either run it on Bochs or Qemu.

### Qemu
```
make run
```

To log registers and cpu execeptions during execution, use `log-run`. The data is then written to `logs.txt`.
```
make log-run
```

### Bochs 
```
bochs
```

### Architecture Records
Diagrams can be found under the doc directory.
