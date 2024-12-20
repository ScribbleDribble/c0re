
global ctx_switch
global switch_to_userspace
global update_esp0

extern USTACK_BASE
extern kstack_save
extern target_esp0
extern prev_esp0
extern userspace_test
extern _set_sysenter_esp


ctx_switch:
    xchg bx, bx

    ; prepares kernel stack for the state save
    ; places required number of registers needed for iret in prev_esp0
    push eax ; save state of registers
    push ebx
    mov eax, [prev_esp0]
    add esp, 8
    sub eax, esp 
    mov ebx, [prev_esp0]
    mov [ebx], eax  ; *prev_esp0 =  prev_esp0 - esp0
    sub esp, 8
    pop ebx
    pop eax

    ; save state of cur process
    pusha
      
    ; switch stacks - target_esp0 will point to the top of all saved registers
    mov esp, [target_esp0]
    sub esp, [esp]
    sub esp, 32
    ;xchg bx, bx
    ; restore state
    popa 

    push esp
    call _set_sysenter_esp ; ensure that syscalls will execute in process' kernel stack
    pop esp
    ; make sure we have ds, eflags, cs, eip in stack respectively 
    xchg bx, bx
    sti 
    iret 




switch_to_userspace:
	mov ax, (4 * 8) | 3 ; ring 3 data with bottom 2 bits set for ring 3
	mov ds, ax
	mov es, ax 
	mov fs, ax 
	mov gs, ax ; SS is handled by iret
 
	; set up the stack frame iret expects
	mov eax, [USTACK_BASE]
	push (4 * 8) | 3 ; data selector
	push eax ; current esp 
	pushf ; eflags
	push (3 * 8) | 3 ; code selector (ring 3 code with bottom 2 bits set for ring 3)
	push userspace_test ; instruction address to return to
	iret
    

