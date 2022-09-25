
; a segment register is basically an offset that the cpu stores for certain elements of the program. For instance, the data segment stores an offset for data, 
; the cpu will use it to calculate the absolute address of a piece of data, like a label. So yeah, the CPU uses this value automatically.

; there segment registers are:  data segment (ds), code segment (cs), stack segment (ss), extra segment (es - user defined))
; they are useful because we can only reference up to 0xffff when using gp registers, as we are in real protected mode. This only allows us to access up to 65kB of memory access, and this
; is simply not enough for bootstraping an OS. segmentation allows us address more memory - thanks cpu designers. 

; there is one rule though, the cpu will multiply the given offset_address by 16. In essence, this is if we shifted our hex values to the right by one. 
; for example, say i want to have all my data to have an index of 0x7c00 (start of boot sector), then we can set the ds register to be 0x7c0. 
; now if we wanted access a label, say with address 0x120, the cpu will then check the relevant segment register (ds in this case) and then calculate its absolute address
; (0x7c0 * 16 + 0x120)  

; now we can access around 1MB of data (0xffff * 16 + 0xffff)
; In essence  we have (base_offset * 16 + offset_from_base) and the cpu uses that to find the absolute address :)


mov ah, 0x0e

mov bx, 0x7c0 ; unfortunately, we can only move values to segment registers if they come from bx. 
mov ds, bx	; works!

mov al, [some_char]
int 0x10

mov al, [es:some_char] ; you need to explicitly tell the cpu to use the gp segment register, otherwise it will use ds
int 0x10	; fails to print some_char data because base (offset) address has not been set!

mov bx, 0x7c0	
mov es, bx	; here we set the base (offset) address 
mov al, [es:some_char] 
int 0x10	; works!

jmp $

some_char:
db "A"


times 510-($-$$) db 0
dw 0xaa55


