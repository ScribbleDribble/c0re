global _tss_load
global _setup_task
extern userspace_test2


_tss_load:
	mov ax, 5*8
	ltr ax
	ret

; sets up stack for new task (to be used by iret)
; takes new ESP3 as parameter
; DS -> ESP (this should be user stack value) -> EFLAGS -> CS -> code entry point

_setup_task:
	xchg bx, bx

	mov eax, esp
	mov esp, [esp + 4] ; esp is now set to new task's ESP0
	push (4 * 8) | 3 ; data selector
	push esp ; TODO set this to new task's ESP3 
	push 0x202 ; EFLAGS - interrupt enable with reserved bits set
	push (3 * 8) | 3 ; code selector (ring 3 code with bottom 2 bits set for ring 3)
	push userspace_test2 ; instruction address to return to
	pusha 			; simulate register save
	mov esp, eax	; reset esp back to source process ESP0 
	ret

EFLAGS_NEW_PROC dw 0x202