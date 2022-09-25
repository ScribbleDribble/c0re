; NOTE: you need to set -fda flag for qemu to work with hard drive booting 
; so qemu-system-x86 -fda boot_sector.bin 

[org 0x7c00]

; we aim to load 5 sectors to 0x0000(ES):0x9000(BX)

mov [BOOT_DRIVE], dl 	; BIOS stores our boot drive in dl, so we remember it for later

mov bp, 0x8000	; set sp and bp out of the way
mov sp, bp	; way at 0x8000 

; we now want to tell the BIOS where to copy the data in memory. For this we use segment registers
; (to again allow us to access more regions of memory in real mode). 

; The BIOS expects us to use the extra segment register (es) and bx as our "offset from base offset". 
; this is ES:BX

mov bx, 0x9000	; set 0x9000 as offset from es segment register. this will be the start addr for in-memory hdd data.
mov dl, [BOOT_DRIVE]
mov dh, [SECTORS_TO_READ]
call disk_load

mov dx, [0x9000]	; print first loaded word
call print_hex

mov dx, [0x9000 + 512]	; print first word from 2nd loaded sector
call print_hex

jmp $

%include "print_hex.asm"
%include "disk_load.asm"
%include "print_string.asm"

BOOT_DRIVE:
db 0

SECTORS_TO_READ:
db 5

times 510-($-$$) db 0 ; db means define byte - in this create 0 bytes 
dw 0xaa55

; add a sanity check. we will try to print these when we read from disk
times 256 dw 0xdada ; dw means create word- in this case, define 256 words of 0xdada (16-bit each because of protectedmode)
times 256 dw 0xface ; 256*2 = 512 bytes of memory reserved.


