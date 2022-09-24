; we plan to alter a registers for printing, so push a register on the stack and then
; pop once finished


print_char:
	pusha 
	int 0x10
	popa
	ret ; removes return addr from stack
