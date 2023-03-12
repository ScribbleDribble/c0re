#include <gtest/gtest.h>

extern "C" {
    #include "../vmm.h"
    #include <string.h>
    #include <sys/mman.h>
}

class VMMTest : public ::testing::Test {

    virtual void SetUp() {
        int prot = PROT_READ | PROT_WRITE | PROT_EXEC;
        int flags = MAP_PRIVATE | MAP_ANONYMOUS;
        // request OS to map virtual addresses that will be used by our paging code. 
        char* buf1 = (char*) mmap((void*) PD_BASE_ADDR, 0x4C4B400, prot, flags, -1, 0);
        char* buf2 = (char*) mmap((void*) PT_BASE_ADDR, 0x4C4B400, prot, flags, -1, 0);
        
        if (buf1 == MAP_FAILED || buf2 == MAP_FAILED) {
            std::cout << "Map failed" << std::endl;
            exit(0);
        } 
        // vmm init has already created two page tables. so test with a third one. 
    }

};

TEST(PagingTests, TestPDECreate) {
    unsigned int pde = create_pde(1,1,0,0,0,0,0,0, 0xA0000);
    std::stringstream ss;
    ss << std::hex << pde << std::endl;
    std::string res(ss.str());

    EXPECT_EQ(pde, 0xA0003);
}

TEST(PagingTests, TestPTECreate) {

    unsigned int pte = create_pte(1,0,0,0,0,0,0,0,0,0, 0x10000);

    EXPECT_EQ(pte, 0x10001);

}

// maybe parametise test for any pd_index
TEST_F(VMMTest, TestCreatePageTable) {
    vmm_init();

    // test creation of 2nd page directory entry
    int pd_index = 2;
    create_page_table(pd_index);

    // point to third page table
    uint32_t* pt = (uint32_t *) PT_BASE_ADDR;
    pt += MAX_PTE_COUNT * pd_index;

    for(int i = 0; i < 1024; i++) {
        EXPECT_EQ(pt[i], create_pte(0,1,0,0,0,0,0,0,0,0, 0));
    }
}

TEST_F(VMMTest, TestIsPresent) {
    EXPECT_TRUE(IS_PRESENT(create_pte(1,1,0,0,0,0,0,0,0,0, 0x1000)));
    EXPECT_FALSE(IS_PRESENT(create_pte(0,1,0,0,0,0,0,0,0,0, 0x1000)));
}

TEST_F(VMMTest, TestPalloc) {
    vmm_init();
    // test creation of 2nd page directory entry with 3 allocations
    int pd_index = 2;
    int n_allocs = 3; 

    create_page_table(pd_index);
    int allocated_vaddress = (uint32_t) palloc(pd_index, n_allocs);
    EXPECT_EQ(allocated_vaddress, pd_index * PT_SIZE_BYTES);

    // point to third page table
    uint32_t* pt = (uint32_t *) PT_BASE_ADDR;
    pt += MAX_PTE_COUNT * pd_index;
    for(int i = 0; i < MAX_PTE_COUNT; i++) {
        int expected_addr = ALIGN * i + PHYS_BASE;
        if (i < n_allocs){
            EXPECT_EQ(pt[i], create_pte(1,1,0,0,0,0,0,0,0,0, expected_addr));
        } else {
            EXPECT_EQ(pt[i], create_pte(0,1,0,0,0,0,0,0,0,0, 0));
        } 
    }
}

TEST_F(VMMTest, TestPallocAddresses) {
    vmm_init();
    // test creation of 2nd page directory entry with 3 allocations

    int n_allocs = 1;
    int pd_index = KERNEL_PD_INDEX;

    create_page_table(KERNEL_PD_INDEX);
    int allocated_vaddress = palloc(pd_index, n_allocs);
    
    EXPECT_EQ(allocated_vaddress, pd_index * PT_SIZE_BYTES);

    n_allocs = 2;
    allocated_vaddress = palloc(pd_index, n_allocs);
    EXPECT_EQ(allocated_vaddress, pd_index*PT_SIZE_BYTES + PAGE_SIZE) 
        << "Actual: " << std::hex << allocated_vaddress << "\nExpected " << std::hex << pd_index*PT_SIZE_BYTES + PAGE_SIZE;

    allocated_vaddress = palloc(pd_index, n_allocs);

    // three page frames have already been allocated, so expect to start at PAGES_SIZE*3
    EXPECT_EQ( allocated_vaddress, pd_index*PT_SIZE_BYTES + PAGE_SIZE*3) 
        << "Actual: " << std::hex << allocated_vaddress << "\nExpected " << std::hex << pd_index*PT_SIZE_BYTES + PAGE_SIZE*3;


}



TEST_F(VMMTest, TestMemMap) {
    vmm_init();
    
    // -1 since we already have 
    EXPECT_EQ(-1, mem_map(0x0));
    EXPECT_EQ(-1, mem_map(0x200000));
    EXPECT_EQ(-1, mem_map(0x400000));
    EXPECT_EQ(1, mem_map(0x400001));
    EXPECT_EQ(2, mem_map(0x800201));
    EXPECT_EQ(3, mem_map(0xC00201));


}


// write test for kernel page table creation 
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
