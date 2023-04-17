
ASM_SOURCES = $(wildcard cpu/*.asm cpu/multitasking/*.asm drivers/*.asm)
C_SOURCES = $(wildcard kernel/*.c kernel/kmalloc/*.c drivers/*.c cpu/*.c util/*.c)

OBJ = ${C_SOURCES:.c=.o}
ASM_OBJ = $(ASM_SOURCES:.asm=.o)

all : bin/kernel.elf bin/boot_sector.bin bin/myos.bin
	objcopy -O binary bin/kernel.elf bin/kernel.bin
	cat bin/boot_sector.bin bin/kernel.bin > bin/os_image
	cp bin/myos.bin isodir/boot/myos.bin
	cp boot/grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o bin/myos.iso isodir

run-new : bin/myos.bin
	qemu-system-x86_64 -kernel bin/myos.bin

run-iso : bin/myos.iso 
	qemu-system-x86_64 -cdrom bin/myos.iso

run : bin/os_image
	qemu-system-x86_64 -fda bin/os_image


debug: bin/kernel.elf bin/boot_sector.bin
	qemu-system-x86_64 -s -S -fda bin/os_image &
	gdb -ex "target remote localhost:1234" -ex "symbol-file bin/kernel.elf"


# logs interrupts and register state to logs.txt
log-run : bin/myos.bin
	echo "" > logs.txt
	qemu-system-x86_64 -d int -D logs.txt -kernel bin/myos.bin > logs.txt 

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
	nasm kernel/kernel_entry.asm -o kernel/kernel_entry.o -f elf

cpu/irq.o : cpu/irq.c
	i386-elf-gcc --freestanding -g -c $(CFLAGS) -mgeneral-regs-only cpu/irq.c -o cpu/irq.o

cpu/port_io.o: cpu/port_io.c
	i386-elf-gcc --freestanding -g -c $(CFLAGS) cpu/port_io.c -o cpu/port_io.o


cpu/descriptor_table_load.o : cpu/descriptor_table_load.asm cpu/idt.o
	nasm cpu/descriptor_table_load.asm -g -f elf -o cpu/descriptor_table_load.o

cpu/timer.o : cpu/timer.c
	i386-elf-gcc --freestanding -c $(CFLAGS) cpu/timer.c -o cpu/timer.o

cpu/isr_handle.o : cpu/isr_handle.asm
	nasm cpu/isr_handle.asm -g -f elf -o cpu/isr_handle.o

cpu/irq_handle.o : cpu/irq_handle.asm
	nasm cpu/irq_handle.asm -g -f elf -o cpu/irq_handle.o

cpu/idt.o : cpu/idt.c cpu/irq.c
	i386-elf-gcc --freestanding -g -c $(CFLAGS) cpu/idt.c -o cpu/idt.o

cpu/multitasking/tss.o : cpu/multitasking/tss.c
	i386-elf-gcc --freestanding -g -c $(CFLAGS) cpu/multitasking/tss.c -o cpu/multitasking/tss.o

cpu/multitasking/load_tss.o : cpu/multitasking/load_tss.asm
	nasm cpu/multitasking/load_tss.asm -f elf -o cpu/multitasking/load_tss.o

kernel/vmm.o: kernel/vmm.c
	i386-elf-gcc --freestanding -g -c $(CFLAGS) kernel/vmm.c -o kernel/vmm.o

kernel/pmm.o: kernel/pmm.c
	i386-elf-gcc --freestanding -g -c $(CFLAGS) kernel/pmm.c -o kernel/pmm.o

kernel/kmalloc.o: kernel/kmalloc/kmalloc.c
	i386-elf-gcc --freestanding -g -c $(CFLAGS) kernel/kmalloc/kmalloc.c -o kernel/kmalloc.o


util/strmap.o: util/strmap.c
	i386-elf-gcc --freestanding -g -c $(CFLAGS) util/strmap.c -o strmap.o

kernel/enable_paging.o: kernel/enable_paging.asm
	nasm kernel/enable_paging.asm -g -f elf -o kernel/enable_paging.o


cpu/gdt.o : cpu/gdt.c 
	i386-elf-gcc --freestanding -g -c $(CFLAGS) cpu/gdt.c -o cpu/gdt.o

user/surface.o : user/surface.c 
	i386-elf-gcc --freestanding -g -c $(CFLAGS) user/surface.c -o user/surface.o


# links ELF kernel_entry and kernel for direct access to main function of kernel. 
# the binary will start at address 0x1000
# bin/kernel.bin : kernel/kernel.o kernel/kernel_entry.o kernel/string.o drivers/vga.o cpu/descriptor_table_load.o cpu/idt.o cpu/isr_handle.o cpu/irq_handle.o cpu/irq.o cpu/port_io.o cpu/timer.o drivers/ps2.o drivers/keyboard.o drivers/driver_entry.o kernel/vmm.o kernel/pmm.o kernel/enable_paging.o kernel/kmalloc.o
# 	i386-elf-ld kernel/kernel.o kernel/kernel_entry.o kernel/string.o drivers/vga.o cpu/descriptor_table_load.o cpu/idt.o cpu/isr_handle.o cpu/irq.o cpu/port_io.o cpu/irq_handle.o cpu/timer.o drivers/ps2.o drivers/keyboard.o drivers/driver_entry.o kernel/vmm.o kernel/pmm.o kernel/enable_paging.o kernel/kmalloc.o -Ttext 0x1000 -o bin/kernel.bin --oformat binary


bin/kernel.elf : kernel/kernel.o kernel/kernel_entry.o kernel/string.o drivers/vga.o cpu/descriptor_table_load.o cpu/idt.o cpu/isr_handle.o cpu/irq_handle.o cpu/irq.o cpu/port_io.o cpu/timer.o drivers/ps2.o drivers/keyboard.o drivers/driver_entry.o kernel/vmm.o kernel/pmm.o kernel/enable_paging.o kernel/kmalloc.o util/strmap.o cpu/gdt.o cpu/multitasking/load_tss.o cpu/multitasking/tss.o user/surface.o 
	 i386-elf-ld kernel/kernel.o kernel/kernel_entry.o kernel/string.o drivers/vga.o cpu/descriptor_table_load.o cpu/idt.o cpu/isr_handle.o cpu/irq.o cpu/port_io.o cpu/irq_handle.o cpu/timer.o drivers/ps2.o drivers/keyboard.o drivers/driver_entry.o kernel/vmm.o kernel/pmm.o kernel/enable_paging.o kernel/kmalloc.o  util/strmap.o cpu/gdt.o cpu/multitasking/load_tss.o cpu/multitasking/tss.o  user/surface.o -Ttext 0x1000 -o bin/kernel.elf

# bin/kernel.elf : ${OBJ} ${ASM_OBJ}
# 	 i386-elf-ld ${OBJ} ${ASM_OBJ} -Ttext 0x1000 -o bin/kernel.elf

boot/boot.o: boot/boot.s
	i386-elf-as boot/boot.s -o boot/boot.o

bin/myos.bin : boot/boot.o kernel/kernel.o kernel/string.o drivers/vga.o cpu/descriptor_table_load.o cpu/idt.o cpu/isr_handle.o cpu/irq_handle.o cpu/irq.o cpu/port_io.o cpu/timer.o drivers/ps2.o drivers/keyboard.o drivers/driver_entry.o kernel/vmm.o kernel/pmm.o kernel/enable_paging.o kernel/kmalloc.o util/strmap.o cpu/multitasking/load_tss.o cpu/multitasking/tss.o cpu/gdt.o user/surface.o
	 i386-elf-gcc -T boot/linker.ld -ffreestanding -O2 -nostdlib -o bin/myos.bin boot/boot.o kernel/kernel.o kernel/string.o drivers/vga.o cpu/descriptor_table_load.o cpu/idt.o cpu/isr_handle.o cpu/irq.o cpu/port_io.o cpu/irq_handle.o cpu/timer.o drivers/ps2.o drivers/keyboard.o drivers/driver_entry.o kernel/vmm.o kernel/pmm.o kernel/enable_paging.o util/strmap.o kernel/kmalloc.o cpu/gdt.o user/surface.o cpu/multitasking/load_tss.o cpu/multitasking/tss.o -lgcc

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
