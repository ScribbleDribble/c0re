
; As we now in 32-bit protected mode,  we cannot use functionality provided by the BIOS as those have been written for 16-bit CPUs. So we need to write our own drivers. 
; Video Graphics Array (VGA), is a memory mapped IO that allows us to either display text or graphics on the screen using either mode. VGA has been kindly loaded by the BIOS before, and now we leverage it. 

; In text mode, the dimensions are 80x24. Remember, main memory is single dimensional, so we access memory sequentially - as if it was on one dimension.
; We dont have to worry about colouring individual pixels, we can just supply the ASCII char.
; The base address for VGA text mode 0xb8000 -- text mode uses two addresses (or parameters if you will) 1. ASCII character 2. text colour and background colour and other character attributes.
; As we store both the character and its display attribute continguously, each character will use up two bytes of memory

; EBX points to base address of string, 

[bits 32]

BASE_VIDEO_MEMORY_ADDR equ 0xb8000

; character attribute (white text on black bg)
WHITE_ON_BLACK equ 0x0f


; ebx - base address of string to print
print_str:
	pusha
	mov edx, BASE_VIDEO_MEMORY_ADDR

start: 
	mov ah, WHITE_ON_BLACK
	mov al, [ebx]

	cmp al, 0
	je print_end
	
	mov [edx], ax ; assign 2 bytes of continguous memory to memory mapped region

	add ebx, 1
	add edx, 2	; remember that we are storing the character and then its display attribute on the next byte, so increment by two

	jmp start

print_end:
	popa 
	ret
