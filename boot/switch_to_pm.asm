[bits 16]
switch_to_pm:
	cli	; disable interrupts as interrupts work differently in PM. interrupt-ageddon may happen if we dont
	lgdt [gdt_descriptor]	; load GDT

	; cr0 register is useful for giving us CONTROL over the processor. depending on which bit you set in the register, you activate functionality
	; 1 bit = 1 processor property. e.g. 0th bit when set activates protected mode. 

	mov eax, cr0	; set first bit of cr0 register to activate protected mode 
	or eax, 0x1	
	mov cr0, eax
	
	jmp CODE_SEG:init_pm	; make far jump to flush cpu pipelining

[bits 32]
init_pm:
	mov ax, DATA_SEG
	mov ds, ax    		; segment registers have no use, point them to the new segment descriptor
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov ebp, 0x90000	; update stack position so that its out of the way and near the top 
	mov esp, ebp
	
	call BEGIN_PM		; ...finally
