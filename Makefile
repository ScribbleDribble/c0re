

C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c )

OBJ = ${C_SOURCES:.c=.o }


all : bin/kernel.elf bin/boot_sector.bin
	objcopy -O binary bin/kernel.elf bin/kernel.bin
	cat bin/boot_sector.bin bin/kernel.bin > bin/os_image

run : bin/os_image
	qemu-system-x86_64 -fda bin/os_image

debug-run : bin/os_image
	qemu-system-x86_64 -d int -fda bin/os_image


debug: bin/kernel.elf bin/boot_sector.bin
	qemu-system-x86_64 -s -S -fda bin/os_image &
	gdb -ex "target remote localhost:1234" -ex "symbol-file bin/kernel.elf"


# logs interrupts and register state to logs.txt
log-run : bin/os_image
	echo "" > logs.txt
	qemu-system-x86_64 -d int -D logs.txt -fda bin/os_image > logs.txt 

# kernel runs with debug msgs
	

CFLAGS += -Wall -Wextra -Wpedantic \
          -Wformat=2 -Wno-unused-parameter -Wshadow \
          -Wwrite-strings -Wstrict-prototypes  \
          -Wredundant-decls -Wnested-externs -Wmissing-include-dirs

# GCC warnings that Clang doesn't provide:
ifeq ($(CC),gcc)
    CFLAGS += -Wjump-misses-init -Wlogical-op
endif

drivers/driver_entry.o : drivers/driver_entry.c
	i386-elf-gcc --freestanding -c -g $(CFLAGS) drivers/driver_entry.c -o drivers/driver_entry.o

drivers/vga.o : drivers/vga.c
	i386-elf-gcc --freestanding -g -c $(CFLAGS) drivers/vga.c -o drivers/vga.o

drivers/ps2.o : drivers/ps2.c 
	i386-elf-gcc --freestanding -g -c $(CFLAGS) drivers/ps2.c -o drivers/ps2.o


drivers/keyboard.o : drivers/keyboard.c
	i386-elf-gcc --freestanding -g -c $(CFLAGS) drivers/keyboard.c -o drivers/keyboard.o

kernel/string.o : kernel/string.c 
	i386-elf-gcc --freestanding -g -c $(CFLAGS) kernel/string.c -o kernel/string.o

kernel/kernel.o : kernel/kernel.c
	i386-elf-gcc --freestanding -g $(CFLAGS) -c kernel/kernel.c -o kernel/kernel.o

kernel/kernel_entry.o : kernel/kernel_entry.asm
	nasm kernel/kernel_entry.asm -f elf -o kernel/kernel_entry.o

cpu/irq.o : cpu/irq.c
	i386-elf-gcc --freestanding -g -c $(CFLAGS) -mgeneral-regs-only cpu/irq.c -o cpu/irq.o

cpu/port_io.o: cpu/port_io.c
	i386-elf-gcc --freestanding -g -c $(CFLAGS) cpu/port_io.c -o cpu/port_io.o


cpu/idt_load.o : cpu/idt_load.asm cpu/idt.o
	nasm cpu/idt_load.asm -f elf -o cpu/idt_load.o

cpu/timer.o : cpu/timer.c
	i386-elf-gcc --freestanding -c $(CFLAGS) cpu/timer.c -o cpu/timer.o

cpu/isr_handle.o : cpu/isr_handle.asm
	nasm cpu/isr_handle.asm -f elf -o cpu/isr_handle.o

cpu/irq_handle.o : cpu/irq_handle.asm
	nasm cpu/irq_handle.asm -f elf -o cpu/irq_handle.o

cpu/idt.o : cpu/idt.c cpu/irq.c
	i386-elf-gcc --freestanding -g -c $(CFLAGS) cpu/idt.c -o cpu/idt.o

kernel/vmm.o: kernel/vmm.c
	i386-elf-gcc --freestanding -g -c $(CFLAGS) kernel/vmm.c -o kernel/vmm.o

kernel/pmm.o: kernel/pmm.c
	i386-elf-gcc --freestanding -g -c $(CFLAGS) kernel/pmm.c -o kernel/pmm.o

kernel/kmalloc.o: kernel/kmalloc/kmalloc.c
	i386-elf-gcc --freestanding -g -c $(CFLAGS) kernel/kmalloc/kmalloc.c -o kernel/kmalloc.o


util/strmap.o: util/strmap.c
	i386-elf-gcc --freestanding -g -c $(CFLAGS) util/strmap.c -o strmap.o

kernel/enable_paging.o: kernel/enable_paging.asm
	nasm kernel/enable_paging.asm -f elf -o kernel/enable_paging.o

# links ELF kernel_entry and kernel for direct access to main function of kernel. 
# the binary will start at address 0x1000
# bin/kernel.bin : kernel/kernel.o kernel/kernel_entry.o kernel/string.o drivers/vga.o cpu/idt_load.o cpu/idt.o cpu/isr_handle.o cpu/irq_handle.o cpu/irq.o cpu/port_io.o cpu/timer.o drivers/ps2.o drivers/keyboard.o drivers/driver_entry.o kernel/vmm.o kernel/pmm.o kernel/enable_paging.o kernel/kmalloc.o
# 	i386-elf-ld kernel/kernel.o kernel/kernel_entry.o kernel/string.o drivers/vga.o cpu/idt_load.o cpu/idt.o cpu/isr_handle.o cpu/irq.o cpu/port_io.o cpu/irq_handle.o cpu/timer.o drivers/ps2.o drivers/keyboard.o drivers/driver_entry.o kernel/vmm.o kernel/pmm.o kernel/enable_paging.o kernel/kmalloc.o -Ttext 0x1000 -o bin/kernel.bin --oformat binary


bin/kernel.elf : kernel/kernel.o kernel/kernel_entry.o kernel/string.o drivers/vga.o cpu/idt_load.o cpu/idt.o cpu/isr_handle.o cpu/irq_handle.o cpu/irq.o cpu/port_io.o cpu/timer.o drivers/ps2.o drivers/keyboard.o drivers/driver_entry.o kernel/vmm.o kernel/pmm.o kernel/enable_paging.o kernel/kmalloc.o util/strmap.o
	 i386-elf-ld kernel/kernel.o kernel/kernel_entry.o kernel/string.o drivers/vga.o cpu/idt_load.o cpu/idt.o cpu/isr_handle.o cpu/irq.o cpu/port_io.o cpu/irq_handle.o cpu/timer.o drivers/ps2.o drivers/keyboard.o drivers/driver_entry.o kernel/vmm.o kernel/pmm.o kernel/enable_paging.o kernel/kmalloc.o  util/strmap.o -Ttext 0x1000 -o bin/kernel.elf

# bin/kernel.elf : kernel/kernel.o kernel/kernel_entry.o kernel/string.o drivers/vga.o cpu/idt_load.o cpu/idt.o cpu/isr_handle.o cpu/irq_handle.o cpu/irq.o cpu/port_io.o cpu/timer.o drivers/ps2.o drivers/keyboard.o drivers/driver_entry.o kernel/vmm.o kernel/pmm.o kernel/enable_paging.o
# 	i386-elf-ld kernel/kernel.o kernel/kernel_entry.o kernel/string.o drivers/vga.o cpu/idt_load.o cpu/idt.o cpu/isr_handle.o cpu/irq.o cpu/port_io.o cpu/irq_handle.o cpu/timer.o drivers/ps2.o drivers/keyboard.o drivers/driver_entry.o kernel/vmm.o kernel/pmm.o kernel/enable_paging.o -Ttext 0x1000 -o bin/kernel.elf 

bin/boot_sector.bin : boot/boot_sector.asm
	cd boot; nasm boot_sector.asm -f bin -o ../bin/boot_sector.bin

clean : 
	rm kernel/*.o
	rm cpu/*.o
	rm drivers/*.o
	rm bin/*

rebuild:
	clear
	$(MAKE) clean
	$(MAKE) all
