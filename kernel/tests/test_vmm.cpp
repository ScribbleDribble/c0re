#include <gtest/gtest.h>

extern "C" {
    #include "../vmm.h"
    #include <string.h>
}


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

// write test for ID mapping 

// write test for kernel page table creation 


int main(int argc, char **argv) {

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
