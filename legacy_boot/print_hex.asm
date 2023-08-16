print_hex:
	pusha
	mov cx, 0 ; this is our loop variable


; say we have the hex 0x2468. We want to convert this hex into ascii chars. 
; ascii chars of digits range from 0x30 to 0x39. To make the conversion, we need to add 0x30 to each number 
digit_to_ascii:
	cmp cx, 4 
	je end_loop
	
	mov ax, dx	; we will use ax for holding the values as that is the accumulator. 
	and ax, 0x000f  ; mask out all digits of ax but keep last hex value
	add al, 0x30	
	cmp al, 0x39	; check to see if numerical digit. if not then we just add 8 to get ascii A-F value 
	jle set_mem_addr
	add al, 7 ; 

; we now want to place our ascii value in the right address for the return. 
; so we need to calculate this address and find the offset for the character from the base address, according to its position on the hex number. 
set_mem_addr:
	
	mov bx, HEX_OUT + 5 ; base addr for string + length of string
	sub bx, cx	; find correct offset by using loop variable
	mov [bx], al	; use indirect addressing to place value in address.
	
	ror dx, 4	; 0x2468 --> 0x8246 -> 0x6824 -> 0x4682. ensures we get 2 from our AND masking.
	add cx, 1
	
	jmp digit_to_ascii

end_loop:
	mov bx, HEX_OUT
	call print_string 
	
	popa
	ret


; reserve memory for new string
HEX_OUT:
	db '0x0000', 0
