[bits 16]

print_string:
    pusha


start:    
    mov al, [bx] ; bx holds base address for string. Now we indirectly access first char value.
    cmp al, 0 ; check if we reach the null-terminator
    je end
    
    mov ah, 0x0e
    int 0x10
    add bx, 1
    
    jmp start
	
    
end:
    popa
    ret
    
