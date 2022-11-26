#include <gtest/gtest.h>

extern "C" {
    #include "../string.h"
}

class IntegerToStringParamTests : public ::testing::TestWithParam<int> {
    
};

// will break for -ve numbers!
TEST_P(IntegerToStringParamTests, TestIntToString) { 
    int num = GetParam();

     
    char expected[32];
    char actual[32];

    memory_set(expected, 0, sizeof(expected));
    memory_set(actual, 0, sizeof(actual));

    sprintf(expected, "%d", num);
    int_to_str(num, actual, 32);

    for (int i = 0; i < 32; ++i) {
        EXPECT_EQ(actual[i], expected[i]) << "Vectors x and y differ at index " << i;
    }
    puts(actual);
}


INSTANTIATE_TEST_CASE_P(
        tests,
        IntegerToStringParamTests,
        ::testing::Values(
    0,1,10,123,421221, 999, 000, -1, -20
    )
);

int main(int argc, char **argv) {

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
