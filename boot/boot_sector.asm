
; tell assmebler preprocessor to automatically set the base address for labels to start at 0x7c00 (start of bootsector) and not 0x0000. Why? Boot sector code starts at 0x7c00

[org 0x7c00]

mov ah, 0x0e 
mov al, "M"
int 0x10 
mov al, "I"
int 0x10
mov al, "C"
int 0x10
mov al, "A"
int 0x10
mov al, [some_char] ; access value held by pointer (indirect addressing)
int 0x10


jmp $

some_char:
	db "H" ; if we were to access this address, we would get just its offset (25 bytes). 25 bytes in main memory would point to somewhere in the IVT. So we would need to either use a preprocessor directive to adjust the base address automatically or manually account for where this code lives in main memory every time.

times 510-($-$$) db 0	; pad out machine instructions with 0s from 

dw 0xaa55	; magic number telling bios that disk is bootable
