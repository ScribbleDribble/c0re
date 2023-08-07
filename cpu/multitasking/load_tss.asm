global _tss_load
global _setup_task
extern userspace_test


_tss_load:
	mov ax, 5*8
	ltr ax
	ret

; sets up stack for new task (to be used by iret)
; takes new ESP3, ESP0, and EIP as args
; DS -> ESP (this should be user stack value) -> EFLAGS -> CS -> code entry point

; TODO - use data and code selector from parent process
_setup_task:
	xchg bx, bx 
	mov eax, esp
	mov edi, [esp + 12]
	mov esi, [esp + 4]
	mov esp, [esp + 8] ; esp is now set to new task's ESP0
	push 0x10 ; data selector - TEMP SOLUTION AS WE MUST GET DS FROM PARENT PROCESS
	push esi 
	push 0x202 ; EFLAGS - interrupt enable with reserved bits set
	push 0x8 ; code selector - TEMP SOLUTION AS WE MUST GET CS FROM PARENT PROCESS 
	push edi ; instruction address to return to
	pusha 			; simulate register save
	mov esp, eax	; reset esp back to source process ESP0 
	ret

EFLAGS_NEW_PROC dw 0x202