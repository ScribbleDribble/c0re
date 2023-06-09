ENTRY (_start)

SECTIONS
{
	. += 0x30100000;
	.text ALIGN (4K) : AT (ADDR (.text) - 0x30000000)
	{	*(.multiboot)
		*(.text)
	}
	.rodata ALIGN (4K) : AT (ADDR (.rodata) - 0x30000000)
	{
		*(.rodata)
	}
	.data ALIGN (4K) : AT (ADDR (.data) - 0x30000000)
	{
		*(.data)
	}
	.bss ALIGN (4K) : AT (ADDR (.bss) - 0x30000000)
	{
		*(COMMON)
		*(.bss)
		*(.bootstrap_stack)
	}
}