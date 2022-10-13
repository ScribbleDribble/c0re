kernel.o : kernel/kernel.c
	i386-elf-gcc --freestanding -c kernel/kernel.c -o kernel/kernel.o

kernel_entry.o : boot/kernel_entry.asm
	nasm boot/kernel_entry.asm -f elf -o boot/kernel_entry.o

kernel.bin : kernel/kernel.o boot/kernel_entry.o
	i386-elf-ld boot/kernel_entry.o kernel/kernel.o -Ttext 0x1000 -o bin/kernel.bin --oformat binary

boot_sector.bin : boot/boot_sector.asm
	cd boot; pwd; nasm boot_sector.asm -f bin -o ../bin/boot_sector.bin

os_image : boot_sector.bin kernel.bin
	cat bin/boot_sector.bin bin/kernel.bin > bin/os_image

clean : 
	rm bin/*
	rm kernel/*.o
	rm boot/*.o
