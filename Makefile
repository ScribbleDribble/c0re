
ASM_SOURCES = $(wildcard cpu/*.asm cpu/multitasking/*.asm drivers/*.asm)
C_SOURCES = $(wildcard kernel/*.c kernel/kmalloc/*.c drivers/*.c cpu/*.c util/*.c)

OBJ = ${C_SOURCES:.c=.o}
ASM_OBJ = $(ASM_SOURCES:.asm=.o)

ISO_DIR="isodir/boot/grub"

all : bin/myos.bin
	if [ ! -d $(ISO_DIR) ]; echo $(ISO_DIR) not found, creating directory...; then mkdir -p $(ISO_DIR); fi;

	cp bin/myos.bin isodir/boot/myos.bin
	cp boot/grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o bin/myos.iso isodir

run : bin/myos.bin
	qemu-system-x86_64 -kernel bin/myos.bin

run-iso : bin/myos.iso 
	qemu-system-x86_64 -cdrom bin/myos.iso

# logs interrupts and register state to logs.txt
log-run : bin/myos.bin
	echo "" > logs.txt
	qemu-system-x86_64 -no-reboot -d int -D logs.txt -kernel bin/myos.bin > logs.txt 


# kernel runs with debug msgs
	

CFLAGS += -Wall -Wextra -Wpedantic \
          -Wformat=2 -Wno-unused-parameter -Wshadow \
          -Wwrite-strings -Wstrict-prototypes  \
          -Wredundant-decls -Wnested-externs -Wmissing-include-dirs --freestanding -g -c -DKERNEL_BUILD

ASMFLAGS += -g -f elf

# GCC warnings that Clang doesn't provide:
ifeq ($(CC),gcc)
    CFLAGS += -Wjump-misses-init -Wlogical-op
endif

drivers/driver_entry.o : drivers/driver_entry.c
	i386-elf-gcc  $(CFLAGS) drivers/driver_entry.c -o drivers/driver_entry.o

drivers/vga.o : drivers/vga.c
	i386-elf-gcc  $(CFLAGS) drivers/vga.c -o drivers/vga.o

drivers/ps2.o : drivers/ps2.c 
	i386-elf-gcc  $(CFLAGS) drivers/ps2.c -o drivers/ps2.o

drivers/serial_io.o : drivers/serial_io.c 
	i386-elf-gcc  $(CFLAGS) drivers/serial_io.c -o drivers/serial_io.o

drivers/keyboard.o : drivers/keyboard.c
	i386-elf-gcc  $(CFLAGS) drivers/keyboard.c -o drivers/keyboard.o

kernel/string.o : kernel/string.c 
	i386-elf-gcc  $(CFLAGS) kernel/string.c -o kernel/string.o

kernel/kernel.o : kernel/kernel.c
	i386-elf-gcc $(CFLAGS) kernel/kernel.c -o kernel/kernel.o


cpu/irq.o : cpu/irq.c
	i386-elf-gcc  $(CFLAGS) -mgeneral-regs-only cpu/irq.c -o cpu/irq.o

cpu/port_io.o: cpu/port_io.c
	i386-elf-gcc  $(CFLAGS) cpu/port_io.c -o cpu/port_io.o


cpu/descriptor_table_load.o : cpu/descriptor_table_load.asm cpu/idt.o
	nasm cpu/descriptor_table_load.asm $(ASMFLAGS) -o cpu/descriptor_table_load.o

cpu/timer.o : cpu/timer.c
	i386-elf-gcc --freestanding -c $(CFLAGS) cpu/timer.c -o cpu/timer.o

cpu/isr_handle.o : cpu/isr_handle.asm
	nasm cpu/isr_handle.asm $(ASMFLAGS) -o cpu/isr_handle.o

cpu/irq_handle.o : cpu/irq_handle.asm
	nasm cpu/irq_handle.asm $(ASMFLAGS) -o cpu/irq_handle.o

cpu/idt.o : cpu/idt.c cpu/irq.c
	i386-elf-gcc  $(CFLAGS) cpu/idt.c -o cpu/idt.o

cpu/multitasking/tss.o : cpu/multitasking/tss.c
	i386-elf-gcc  $(CFLAGS) cpu/multitasking/tss.c -o cpu/multitasking/tss.o

cpu/multitasking/load_tss.o : cpu/multitasking/load_tss.asm
	nasm cpu/multitasking/load_tss.asm -f elf -o cpu/multitasking/load_tss.o

kernel/vmm.o: kernel/vmm.c
	i386-elf-gcc  $(CFLAGS) kernel/vmm.c -o kernel/vmm.o

kernel/pmm.o: kernel/pmm.c
	i386-elf-gcc  $(CFLAGS) kernel/pmm.c -o kernel/pmm.o

kernel/kmalloc.o: kernel/kmalloc/kmalloc.c
	i386-elf-gcc  $(CFLAGS) kernel/kmalloc/kmalloc.c -o kernel/kmalloc.o

common/panic.o: common/panic.c 
	i386-elf-gcc  $(CFLAGS) common/panic.c -o common/panic.o


util/strmap.o: util/strmap.c
	i386-elf-gcc  $(CFLAGS) util/strmap.c -o util/strmap.o

util/priority_queue.o: util/priority_queue.c 
	i386-elf-gcc  $(CFLAGS) util/priority_queue.c -o util/priority_queue.o

kernel/syscall_handler.o: kernel/syscall_handler.asm
	nasm kernel/syscall_handler.asm $(ASMFLAGS) -o kernel/syscall_handler.o

kernel/process.o: kernel/process.c 
	i386-elf-gcc  $(CFLAGS) kernel/process.c -o kernel/process.o

kernel/scheduler.o: kernel/scheduler.c
	i386-elf-gcc  $(CFLAGS) kernel/scheduler.c -o kernel/scheduler.o

user/stdio.o: user/stdio.c
	i386-elf-gcc  $(CFLAGS) user/stdio.c -o user/stdio.o

user/sys/proc.o: user/sys/proc.c
	i386-elf-gcc $(CFLAGS) user/sys/proc.c -o user/sys/proc.o

user/syscall.o: user/syscall.asm
	nasm user/syscall.asm $(ASMFLAGS) -o user/syscall.o

cpu/multitasking/context_switch.o: cpu/multitasking/context_switch.asm
	nasm cpu/multitasking/context_switch.asm $(ASMFLAGS) -o cpu/multitasking/context_switch.o

cpu/gdt.o : cpu/gdt.c 
	i386-elf-gcc  $(CFLAGS) cpu/gdt.c -o cpu/gdt.o

user/surface.o : user/surface.c 
	i386-elf-gcc  $(CFLAGS) user/surface.c -o user/surface.o

boot/boot.o: boot/boot.s
	i386-elf-as boot/boot.s -o boot/boot.o

bin/myos.bin : boot/boot.o kernel/kernel.o kernel/string.o drivers/vga.o cpu/descriptor_table_load.o cpu/idt.o cpu/isr_handle.o cpu/irq_handle.o cpu/irq.o cpu/port_io.o cpu/timer.o drivers/ps2.o drivers/keyboard.o drivers/driver_entry.o kernel/vmm.o kernel/pmm.o  kernel/kmalloc.o util/strmap.o cpu/multitasking/load_tss.o cpu/multitasking/tss.o cpu/gdt.o user/surface.o kernel/process.o kernel/scheduler.o cpu/multitasking/context_switch.o user/syscall.o user/stdio.o kernel/syscall_handler.o drivers/serial_io.o common/panic.o user/sys/proc.o util/priority_queue.o
	 i386-elf-gcc -T boot/linker.ld -ffreestanding -O2 -nostdlib -o bin/myos.bin boot/boot.o kernel/kernel.o kernel/string.o drivers/vga.o cpu/descriptor_table_load.o cpu/idt.o cpu/isr_handle.o cpu/irq.o cpu/port_io.o cpu/irq_handle.o cpu/timer.o drivers/ps2.o drivers/keyboard.o drivers/driver_entry.o kernel/vmm.o kernel/pmm.o  util/strmap.o kernel/kmalloc.o cpu/gdt.o user/surface.o cpu/multitasking/load_tss.o cpu/multitasking/tss.o kernel/process.o kernel/scheduler.o cpu/multitasking/context_switch.o user/syscall.o user/stdio.o  user/sys/proc.o kernel/syscall_handler.o drivers/serial_io.o common/panic.o util/priority_queue.o -lgcc

clean : 
	rm kernel/*.o
	rm cpu/*.o
	rm drivers/*.o
	rm bin/*

rebuild:
	clear
	$(MAKE) clean
	$(MAKE) all
