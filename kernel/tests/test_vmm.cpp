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
    std::stringstream ss;
    ss << std::hex << pte << std::endl;
    std::string res(ss.str());

    EXPECT_EQ(pte, 0x10001);

}

// maybe parametise test for any pd_index
TEST_F(VMMTest, TestCreatePageTable) {
    vmm_init();
    // get current tail so we can replay kalloc calls
    uint32_t tail = pmm_get_bitmap_tail();

    // test creation of 2nd page directory entry
    int pd_index = 2;
    create_page_table(pd_index);
    int expected_kalloc_addr = tail * ALIGN + PHYS_BASE;

    // point to third page table
    uint32_t* pt = (uint32_t *) PT_BASE_ADDR;
    pt += MAX_PTE_COUNT * pd_index;

    for(int i = 0; i < 1024; i++) {
        EXPECT_EQ(pt[i], create_pte(1,1,0,0,0,0,0,0,0,0, expected_kalloc_addr));
        expected_kalloc_addr += ALIGN;
    }
}

// write test for kernel page table creation 
int main(int argc, char **argv) {

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
