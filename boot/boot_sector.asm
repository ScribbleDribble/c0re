; as you may know, a function call is essentially a jmp instruction to a label, executing some code
; ... then jumping where we were before. This is not scalable -- if we call one function 
; in different places of our program, we can only jump back to one label in the program.
; what we can do is use functions kindly given by the cpu -- call and ret. 
; call allows us to save the program location of the calling function.
; ret will take us back to caller's location.

; for func args, we need to push these args to a stack. why we cannot use registers? 
; ...because this introduces side effects, such as modifying the calling funcs data that they may need

[org 0x7c00]

mov al, 0x20
cmp al, 0x20

je then_block
mov al, "A"
int 0x10
jmp the_end

then_block:
	mov al, "B"
	int 0x10
	jmp the_end	

the_end:
	pusha 
	call print_char
	popa

jmp $ 

print_char:
	mov ah, 0x0e; 
	mov al, bl	
	int 0x10; 
	ret ; restore state of registers for preceding functions in stack 



times 510-($-$$) db 0
dw 0xaa55

