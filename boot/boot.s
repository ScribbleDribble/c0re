/* Declare constants for the multiboot header. */
.set ALIGN,    1<<0             /* align loaded modules on page boundaries */
.set MEMINFO,  1<<1             /* provide memory map */
.set FLAGS,    ALIGN | MEMINFO  /* this is the Multiboot 'flag' field */
.set MAGIC,    0x1BADB002       /* 'magic number' lets bootloader find the header */
.set CHECKSUM, -(MAGIC + FLAGS) /* checksum of above, to prove we are multiboot */

/* 
Declare a multiboot header that marks the program as a kernel. These are magic
values that are documented in the multiboot standard. The bootloader will
search for this signature in the first 8 KiB of the kernel file, aligned at a
32-bit boundary. The signature is in its own section so the header can be
forced to be within the first 8 KiB of the kernel file.
*/
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM



 
/*
The multiboot standard does not define the value of the stack pointer register
(esp) and it is up to the kernel to provide a stack. This allocates room for a
small stack by creating a symbol at the bottom of it, then allocating 16384
bytes for it, and finally creating a symbol at the top. The stack grows
downwards on x86. The stack is in its own section so it can be marked nobits,
which means the kernel file is smaller because it does not contain an
uninitialized stack. The stack on x86 must be 16-byte aligned according to the
System V ABI standard and de-facto extensions. The compiler will assume the
stack is properly aligned and failure to align the stack will result in
undefined behavior.
*/
.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:
 
.section .bss, "aw", @nobits

.align 0x1000
boot_page_dir:
	.skip 0x1000
.align 0x1000
boot_page_table:
	.skip 0x1000

# map page table when


/*
The linker script specifies _start as the entry point to the kernel and the
bootloader will jump to this position once the kernel has been loaded. It
doesn't make sense to return from this function as the bootloader is gone.
*/
.section .text
.global _start
.type _start, @function
_start:
	/*
	The bootloader has loaded us into 32-bit protected mode on a x86
	machine. Interrupts are disabled. Paging is disabled. The processor
	state is as defined in the multiboot standard. The kernel has full
	control of the CPU. The kernel can only make use of hardware features
	and any code it provides as part of itself. There's no printf
	function, unless the kernel provides its own <stdio.h> header and a
	printf implementation. There are no security restrictions, no
	safeguards, no debugging mechanisms, only what the kernel provides
	itself. It has absolute and complete power over the
	machine.
	*/
 
	/*
	To set up a stack, we set the esp register to point to the top of the
	stack (as it grows downwards on x86 systems). This is necessarily done
	in assembly as languages such as C cannot function without a stack.
	*/
	mov $(0x30150000), %esp


	/*
	This is a good place to initialize crucial processor state before the
	high-level kernel is entered. It's best to minimize the early
	environment where crucial features are offline. Note that the
	processor is not fully initialized yet: Features such as floating
	point instructions and instruction set extensions are not initialized
	yet. The GDT should be loaded here. Paging should be enabled here.
	C++ features such as global constructors and exceptions will require
	runtime support to work as well.
	*/

	_init_page_table:
	mov $(boot_page_table - 0x30000000), %eax 
	mov $0, %ecx 
	mov $0, %ebx /* cur page frame offset */
	mov $0, %edx /** offset index **/
	mov $((boot_page_table + 0x30000*4) - 0x30000000), %edi /** kernel offset **/

	/* point to start of kernel offset in page table */
	/* add %eax, %edi */

	_fill_page_table:
		cmp $(1024), %ecx
		je _init_page_directory

		cmp $(0x400000), %ebx
		jge _init_page_directory

		jmp _kernel_higher_half_and_identity_mapping


		_kernel_higher_half_and_identity_mapping:
		/* higher half mapping */
		mov %ebx, (%edi) 
		or $7, (%edi)

		/* ID mapping */
		mov %ebx, (%eax) 
		or $7, (%eax)	

		add $(0x1000), %ebx
		add $4, %edi
		add $4, %eax
		inc %ecx 
		jmp _fill_page_table

	_init_page_directory:	
	mov $(0), %ecx
	mov $(boot_page_dir-0x30000000), %eax	
	mov $(boot_page_table-0x30000000), %ebx	

	_fill_page_directory:
		cmp $(1024), %ecx
		je _finalise_page_table

		cmp $(0), %ecx
		je _set_present_page_table_entries
		
		cmp $(0xC0), %ecx
		je _set_present_page_table_entries

			mov %ebx, (%eax)
			inc %ecx
			add $(1024*4), %ebx
			add $(4), %eax
			jmp _fill_page_directory
		
		_set_present_page_table_entries:
			or $(7), %ebx
			mov %ebx, (%eax)
			xor $(7), %ebx 
			inc %ecx
			add $(1024*4), %ebx
			add $4, %eax
			jmp _fill_page_directory


	_finalise_page_table:
		mov $(boot_page_dir-0x30000000), %eax
		mov %eax, %cr3
		mov %cr0, %eax 
		or $(0x80000001), %eax
		mov %eax, %cr0 
		
/* 	
	store address of start of text section using the lea instruction which loads the address of a label. 
	In our linker script, we defined the base address of .section .text as (0x30100000)
	
	now we get to the reason why we identity mapped our kernel earlier. 
	We are still executing instructions in lower addressable memory (because we haven't jumped yet and you wouldn't jump before enabling paging)
	This is why we needed to ID map the kernel

*/  
	lea 4f, %ecx 
	jmp *%ecx  

.section .text
4:
	/*
	Enter the high-level kernel. The ABI requires the stack is 16-byte
	aligned at the time of the call instruction (which afterwards pushes
	the return pointer of size 4 bytes). The stack was originally 16-byte
	aligned above and we've pushed a multiple of 16 bytes to the
	stack since (pushed 0 bytes so far), so the alignment has thus been
	preserved and the call is well defined.
	*/

	/* remove ID mapped kernel as we no longer need it */
	mov $(boot_page_dir), %eax
	mov (%eax), %edx
	and $(0xfffff000), %edx
	mov %edx, (%eax)
	mov %cr3, %ecx
	mov %ecx, %cr3

	/* enable 4MB pages */
	mov %cr4, %eax 
	or $(0x00000010), %eax
	mov %eax, %cr4

	push $(boot_page_table)
	push $(boot_page_dir)

	call kmain
 
	/*
	If the system has nothing more to do, put the computer into an
	infinite loop. To do that:
	1) Disable interrupts with cli (clear interrupt enable in eflags).
	   They are already disabled by the bootloader, so this is not needed.
	   Mind that you might later enable interrupts and return from
	   kernel_main (which is sort of nonsensical to do).
	2) Wait for the next interrupt to arrive with hlt (halt instruction).
	   Since they are disabled, this will lock up the computer.
	3) Jump to the hlt instruction if it ever wakes up due to a
	   non-maskable interrupt occurring or due to system management mode.
	*/
	cli
1:	hlt
	jmp 1b
 
/*
Set the size of the _start symbol to the current location '.' minus its start.
This is useful when debugging or when you implement call tracing.
*/
.size _start, . - _start

