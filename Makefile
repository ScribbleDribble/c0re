
all : bin/boot_sector.bin bin/kernel.bin
	cat bin/boot_sector.bin bin/kernel.bin > bin/os_image

run : bin/os_image
	qemu-system-x86_64 -fda bin/os_image 

drivers/screen.o : drivers/screen.c
	i386-elf-gcc --freestanding -c drivers/screen.c -o drivers/screen.o

kernel/string.o : kernel/string.c 
	i386-elf-gcc --freestanding -c kernel/string.c -o kernel/string.o

kernel/kernel.o : kernel/kernel.c
	i386-elf-gcc --freestanding -c kernel/kernel.c -o kernel/kernel.o

kernel/kernel_entry.o : kernel/kernel_entry.asm
	nasm kernel/kernel_entry.asm -f elf -o kernel/kernel_entry.o

# links ELF kernel_entry and kernel for direct access to main function of kernel. 
# the binary will start at address 0x1000
bin/kernel.bin : kernel/kernel.o kernel/kernel_entry.o kernel/string.o drivers/screen.o 
	i386-elf-ld kernel/kernel.o kernel/kernel_entry.o kernel/string.o drivers/screen.o -Ttext 0x1000 -o bin/kernel.bin --oformat binary

bin/boot_sector.bin : boot/boot_sector.asm
	cd boot; nasm boot_sector.asm -f bin -o ../bin/boot_sector.bin

clean : 
	rm bin/*
	rm kernel/*.o
