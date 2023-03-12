[bits 32]
[extern kmain]   ; tell that we will be referencing this function defined elsewhere
call kmain
jmp $
	
