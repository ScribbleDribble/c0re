
[org 0x7c00]

mov bp, 0x9000
mov sp, bp

mov bx, MSG_REAL_MODE
call print_string

call switch_to_pm

jmp $

%include "gdt.asm"
%include "print_string_vga.asm"
%include "print_string.asm"
%include "switch_to_pm.asm"

[bits 32]

BEGIN_PM:
        mov ebx, MSG_INIT_PROTECTED_MODE
        call print_string_vga
	jmp $	

MSG_REAL_MODE db "Started up 16-bit real mode", 0
MSG_INIT_PROTECTED_MODE db "Booted up 32-bit Protected Mode", 0

times 510-($-$$) db 0
dw 0xaa55
