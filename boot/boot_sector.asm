; -stack grows downward
; -stack has bp and sp and sp points to top of stack. decrements with each push to stack
; -we are still in 16-bit real mode, so our stack only uses 16 bits (chars will be padded with 8-bit 0s)
; we can should retrieve popped values in bx reg, which supports 16bit values. bl will hold char/8-bit info


mov ah, 0x0e

mov bp, 0x8000 ; set bp to be far away from bios or important code
mov sp, bp ; set sp to be same as bp

push "P"
push "M"
push "O"
push "C"

pop bx
mov al, bl ; the lower 8 bits hold the char. higher 8 bits have padded 0s
int 0x10 

pop bx
mov al, bl
int 0x10

pop bx 
mov al, bl
int 0x10

mov al, [0x7ffe] ; this is 0x8000 (bp addr) - 0x2 (16-bit char values stored in stack). 
; we move this into the al reg and display it. this proves that the stack is 16-bit and we move downwards
 
int 0x10

jmp $


times 510-($-$$) db 0	; pad out machine instructions with 0s from 

dw 0xaa55	; magic number telling bios that disk is bootable
