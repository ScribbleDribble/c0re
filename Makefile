
all : bin/boot_sector.bin bin/kernel.bin
	cat bin/boot_sector.bin bin/kernel.bin > bin/os_image

run : bin/os_image
	qemu-system-x86_64 -fda bin/os_image 


drivers/driver_entry.o : drivers/driver_entry.c
	i386-elf-gcc --freestanding -c drivers/driver_entry.c -o drivers/driver_entry.o

drivers/vga.o : drivers/vga.c
	i386-elf-gcc --freestanding -c drivers/vga.c -o drivers/vga.o

drivers/ps2.o : drivers/ps2.c 
	i386-elf-gcc --freestanding -c drivers/ps2.c -o drivers/ps2.o

drivers/keyboard.o : drivers/keyboard.c
	i386-elf-gcc --freestanding -c drivers/keyboard.c -o drivers/keyboard.o

kernel/string.o : kernel/string.c 
	i386-elf-gcc --freestanding -c kernel/string.c -o kernel/string.o

kernel/kernel.o : kernel/kernel.c
	i386-elf-gcc --freestanding -c kernel/kernel.c -o kernel/kernel.o

kernel/kernel_entry.o : kernel/kernel_entry.asm
	nasm kernel/kernel_entry.asm -f elf -o kernel/kernel_entry.o

cpu/irq.o : cpu/irq.c
	i386-elf-gcc --freestanding -c -mgeneral-regs-only cpu/irq.c -o cpu/irq.o

cpu/port_io.o: cpu/port_io.c
	i386-elf-gcc --freestanding -c cpu/port_io.c -o cpu/port_io.o


cpu/idt_load.o : cpu/idt_load.asm cpu/idt.o
	nasm cpu/idt_load.asm -f elf -o cpu/idt_load.o

cpu/timer.o : cpu/timer.c
	i386-elf-gcc --freestanding -c cpu/timer.c -o cpu/timer.o

cpu/isr_handle.o : cpu/isr_handle.asm
	nasm cpu/isr_handle.asm -f elf -o cpu/isr_handle.o

cpu/irq_handle.o : cpu/irq_handle.asm
	nasm cpu/irq_handle.asm -f elf -o cpu/irq_handle.o

cpu/idt.o : cpu/idt.c cpu/irq.c
	i386-elf-gcc --freestanding -c cpu/idt.c -o cpu/idt.o



# links ELF kernel_entry and kernel for direct access to main function of kernel. 
# the binary will start at address 0x1000
bin/kernel.bin : kernel/kernel.o kernel/kernel_entry.o kernel/string.o drivers/vga.o cpu/idt_load.o cpu/idt.o cpu/isr_handle.o cpu/irq_handle.o cpu/irq.o cpu/port_io.o cpu/timer.o drivers/ps2.o drivers/keyboard.o drivers/driver_entry.o
	i386-elf-ld kernel/kernel.o kernel/kernel_entry.o kernel/string.o drivers/vga.o cpu/idt_load.o cpu/idt.o cpu/isr_handle.o cpu/irq.o cpu/port_io.o cpu/irq_handle.o cpu/timer.o drivers/ps2.o drivers/keyboard.o drivers/driver_entry.o -Ttext 0x1000 -o bin/kernel.bin --oformat binary

bin/boot_sector.bin : boot/boot_sector.asm
	cd boot; nasm boot_sector.asm -f bin -o ../bin/boot_sector.bin

bin/os_image:
	cat boot_sector.bin + kernel.bin > os_image

clean : 
	rm bin/*
	rm kernel/*.o
	rm cpu/*.o
	rm drivers/*.o

rebuild:
	clear
	$(MAKE) clean
	$(MAKE) all
