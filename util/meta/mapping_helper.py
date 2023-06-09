INT_SIZE_BYTES = 4

def get_pd_index(virt_base: int):
	return int(virt_base/0x400000)

def get_pte_index(virt_base: int):
	return int(virt_base/0x1000)

def get_pte_address(page_table_addr :int, virt_base :int): 
	return int(page_table_addr+get_pte_index(virt_base)*INT_SIZE_BYTES)

vbase = int(input("Enter the base virtual address of the kernel: \t"), 16)
print(f"Page directory index for kernel {hex(get_pd_index(vbase))}")
print(f"Starting PTE address is {hex(get_pte_index(vbase))}")

