global _tss_load
global _setup_task
extern userspace_test


_tss_load:
	mov ax, 5*8
	ltr ax
	ret




EFLAGS_NEW_PROC dw 0x202