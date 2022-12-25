#include <gtest/gtest.h>

extern "C" {
    #include "../mem_manager.h"
    #include <string.h>
}



TEST(PagingTests, TestPDECreate) {

    unsigned int pde = create_pde(1,1,0,1,0,0,0,1, 0xA00AB);
    std::stringstream ss;
    ss << std::hex << pde << std::endl;
    std::string res(ss.str());

    EXPECT_EQ(pde, 0xA00AB08B);

}

int main(int argc, char **argv) {

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
