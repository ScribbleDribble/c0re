

gdt_start:


gdt_null:
	dd 0x0
	dd 0x0

gdt_code_segment_descriptor: 
	; base = 0x0 limit=0xfffff (maximise limit to conform to flat memory model - we plan on using paging)
	; 1st flags (present)1 (priviledge)00 (descriptor type)1 -> 0b1001
	; type flags (code)1 (conforming)0 (readable)1 (accessed)0 -> 0b1010
	; 2nd flags (granularity)1 (32-bit default)1 (64-bit seg) 0 (AVL) 0 -> 1100
	
	; first double word of segment descriptor 
	dw 0xffff	; Limit (bits 0-15) 
	dw 0x0		; Base (bits 0-15 of base)
	
	; second double word 
	db 0x0		; Base (bits 16-23 of base)
	db 10011010b	; 1st flags and type flags
	db 11001111b 	; 2nd flags, Limit (bits 16-19) 
	db 0x0		; Base (bits 24-31)

gdt_data_segment_descriptor:
	; same as code descriptor except for type flags:
	; type flags: (code) 0 (expand down) 0  (writable) 1 (accessed) 0 -> 0b0010

	; first double word of segment descriptor 
	dw 0xffff	
	dw 0x0		
	
	; second double word 
	db 0x0		
	db 10010010b	
	db 11001111b 	; 2nd flags, Limit (bits 16-19)  
	db 0x0

gdt_end:	; we place a descriptor at the end of the GDT so that we can calculate the size
		; of the descriptor and use this value in our GDT descriptor (below)

gdt_descriptor:
	dw gdt_end - gdt_start - 1	; size of GDT, and is always -1 of true size
	dd gdt_start			; start address of gdt


; We need the following constants so that we tell the CPU which segment register to use 
CODE_SEG equ gdt_code_segment_descriptor - gdt_start
DATA_SEG equ gdt_data_segment_descriptor - gdt_start
