
[org 0x7c00]

KERNEL_OFFSET equ 0x1000

mov bp, 0x9000
mov sp, bp

mov bx, MSG_REAL_MODE
call print_string
call load_kernel
call switch_to_pm

jmp $

%include "gdt.asm"
%include "print_string_vga.asm"
%include "print_string.asm"
%include "switch_to_pm.asm"
%include "disk_load.asm"

[bits 16]
load_kernel:
	mov bx, MSG_LOAD_KERNEL
	call print_string

	; setup parameters for our disk_load function
	mov bx, KERNEL_OFFSET ; value in bx implicitly used by BIOS to load disk data into an offset in memory
	mov dh, [SECTOR_READ_COUNT]		; read first n sectors - arg for disk_load
	mov dl, [BOOT_DRIVE] ; specify which disk drive to read - arg for disk_load
	call disk_load
	ret

[bits 32]
BEGIN_PM:
	mov ebx, MSG_INIT_PROTECTED_MODE
	call print_string_vga

	call KERNEL_OFFSET
	jmp $	

BOOT_DRIVE db 0
SECTOR_READ_COUNT db 25
MSG_LOAD_KERNEL db "Loading kernel", 0
MSG_DEBUG db "HERE", 0
MSG_REAL_MODE db "Started up 16-bit real mode", 0
MSG_INIT_PROTECTED_MODE db "Booted up 32-bit Protected Mode", 0

times 510-($-$$) db 0
dw 0xaa55
