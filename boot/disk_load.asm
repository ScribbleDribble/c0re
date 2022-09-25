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

	mov ah, 0x02	; BIOS read sector function


	; CHS addressing scheme 
	mov dl, 0	; Read drive 0
	mov ch, 3	; Select cylinder 3
	mov dh, 1	; Select track 2 - these are 0 indexed
	mov cl, 4	; select 4th sector on the track - these are 1 indexed

	mov al, SECTOR_READ_AMOUNT 	; read 5 sectors from the start point (4th sector - specified above)


	; we now want to tell the BIOS where to copy the data in memory. For this we use segment registers
	; (to again allow us to access more regions of memory in real mode). 

	; The BIOS expects us to use the extra segment register (es) and bx as our "offset from base offset". 
	; this is ES:BX

	mov bx, 0xa000
	mov es, bx
	mov bx, 0x1234

	; remember, cpu will do (0xa000 * 16 + 0x1234) -> 0xa1234
	int 0x13 ; issue BIOS to perform the read from disk.

	; as with all IO operations, this could fail, (faulty sector, out of bounds, etc).
	; here we perform error checking. BIOS updates registers to signal faults occured.
	
	; carry flag (CF) is set to signal a fault 
	jc disk_error ; jump if carry flag was set
	
	; BIOS will set al as the number of sectors successfully read.
	cmp al, SECTOR_READ_AMOUNT ; remember we requested to read 5 sectors from the start point? how many of those sector reads were successful
	jne disk_error

disk_error:
	mov bx, DISK_ERROR_MSG
	call print_string
	jmp $

DISK_ERROR_MSG: db "Disk read error!", 0
SECTOR_READ_AMOUNT
