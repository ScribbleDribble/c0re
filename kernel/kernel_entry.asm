[bits 32]
[extern main]   ; tell that we will be referencing this function defined elsewhere
call main
jmp $
	
