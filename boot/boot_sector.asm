[bits 32]

[org 0x7c00]

; we cannot use print_string_vga yet as we have not entered protected mode

mov ebx, HELLO_WORLD
call print_str

jmp $

%include "print_string_vga.asm"

HELLO_WORLD:
	db "Hello World!", 0

times 510-($-$$) db 0
dw 0xaa55

