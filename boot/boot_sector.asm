; with x86 arch, we have an ax register for IO and arithmetic. we will be using it for screen IO.
; the ah (higher) will be used for function and al (lower 8 bits) for data.

mov ah, 0x0e ; tell video interrupt what you want to do. we want to draw contents of al  using tty mode. 
mov al, "H" ; lower side of ax register will hold chars to draw on screen
int 0x10 ; raise interrupt to draw "H" on screen
mov al, "E"
int 0x10
mov al, "L"
int 0x10
int 0x10
mov al, "O"
int 0x10

jmp $

times 510-($-$$) db 0	; pad out machine instructions with 0s from 

dw 0xaa55	; magic number telling bios that disk is bootable
