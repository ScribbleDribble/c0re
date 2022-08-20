; as you may know, a function call is essentially a jmp instruction to a label, executing some code
; ... then jumping where we were before. This is not scalable -- if we call one function 
; in different places of our program, we can only jump back to one label in the program.
; what we can do is use functions kindly given by the cpu -- call and ret. 
; call allows us to save the program location of the calling function.
; ret will take us back to caller's location.

; for func args, we can use registers. The caller and callee will decide between them on what registers to use.  but how can we make sure the callee does not overwrite a register of the caller/parent?

; for this we use a special commmand pusha (saves all registers on stack and popa (restore
; all register state)
 
[org 0x7c00]


mov ah, 0x0e

mov al, 0x21
cmp al, 0x20

je then_block
jmp else_block

then_block:
	mov al, "B"
	int 0x10
	jmp the_end	

else_block:
	mov al, "A"
	int 0x10
	jmp the_end

the_end:
	mov al, "C" ; our argument for print_char
	;call print_char ; sets program counter to start of print_char and pushes return addr on stack
	%include "print_string.asm"
	mov bx, GOODBYE_MSG
	call print_string 



jmp $

times 510-($-$$) db 0
dw 0xaa55


; Data
GOODBYE_MSG:
db "Goodbye!", 0
