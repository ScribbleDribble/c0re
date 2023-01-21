
// manages 4kb blocks of memory. 





typedef struct block_header_t {
    uint32_t space : 3;
    uint32_t block_size : 29
}block_header_t;



// 8-bit aligned bh 

block_header_t root_bh;
// 32 block_header_t. contains block size and blocks used (3 bits)
// payload to have requested size in bytes. memset payload base address from 0 to requested_size.
// padding. pad out bits to nearest multiple of 8.
// another 32 block_header_t. contains block size and blocks used (3 bits)
block_header_t create_block_header() {

}

void first_fit() {

}

void* kmalloc(uint32_t size) {
    block_header_t bh = create_block_header();
}

void init_kmalloc() {
    // get first page from kernel page table 
    
}