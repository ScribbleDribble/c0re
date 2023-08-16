; The aim is to now load the kernel into memory, as it wont fit in our boot sector. This is why the boot sector is known to be bootstrap code for the kernel. 
; To access the kernel, which resides on the disk, we need to know how to read using disk drives. 
; The BIOS provides a useful abstraction for data drives (disk, floppy) and we will use them to read. 

; Imagine a platter with a head (like a vinyl player) but several of them stacked together. Thats our hard drive.
; How do we access a specific block of data within stacked platters?

; We can use a 3D esque coordinate system - Cylinder Head Sector (addressing):
 
; Cylinder - Imagine a cylinder clipping through all of the platters. You will then notice a circle that the 
; cylinder forms is a track for all platters. So a cyclinder is the set of tracks for all cylinders.

; Head - this allows us to select an individual platter, as there is usually a 2-1 association of head to platter
; one is head for reading and one for writing.

; Sector - A track is divided by sectors, usually in 512B. So we can select which one here.

; To use the BIOS' interface for accessing the drives, it expects data to be placed in certain registers. 

; https://www.youtube.com/watch?v=d0xn68w3KPE for a visualisation of the disk


disk_load:
	push dx ; dx (data register) is what we used to pass an argument concerning how many sectors to read
	; but we will also need to use it in order to perform a read operation via the BIOS interface.
	; so we will push it to the stack to save it for later use.	

	mov ah, 0x02	; BIOS read sector function
		
	mov al, dh ; read dh amount of sectors from start sector

	; CHS addressing scheme 
	mov ch, 0x00	; Select cylinder 0
	mov dh, 0x00	; Select head 0 - these are 0 indexed
	mov cl, 0x02	; select 2nd sector on the track - these are 1 indexed

	; remember, cpu will do (0xa000 * 16 + 0x1234) -> 0xa1234
	int 0x13 ; issue BIOS to perform the read from disk.

	; as with all IO operations, this could fail, (faulty sector, out of bounds, etc).
	; here we perform error checking. BIOS updates registers to signal faults occured.
	
	; carry flag (CF) is set to signal a fault 
	jc disk_carry_flag_set_err ; jump if carry flag was set
		

	pop dx;

	; BIOS will set al as the number of sectors successfully read.
	cmp al, dh ; remember we requested to read 5 sectors from the start point? how many of those sector reads were successful
	jne disk_sector_read_failure
	ret

disk_carry_flag_set_err:
	mov bx, DISK_CARRY_FLAG_SET_ERR_MSG
	call print_string
	jmp disk_error

disk_sector_read_failure:
	mov bx, DISK_SECTOR_READ_ERR_MSG
 	call print_string
	jmp disk_error

disk_error:
	mov bx, DISK_ERROR_MSG
	call print_string
	jmp $

DISK_ERROR_MSG: db "Disk read error!", 0
DISK_CARRY_FLAG_SET_ERR_MSG: db "Carry flag was set ", 0
DISK_SECTOR_READ_ERR_MSG: db "Failed to read one or more sectors", 0
