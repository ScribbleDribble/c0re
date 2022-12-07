#include <gtest/gtest.h>

extern "C" {
    #include "../string.h"
    #include <string.h>
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
    kputs(actual);
}

TEST(MemoryCopyParamTests, TestMemoryCopy) {
    char some_array1[] = "Hello ";
    char some_array2[] = "Hello World!";
    char *varA, *varB, *varC;
    varA=(char*)malloc(32);
    varB=(char*)malloc(32);
    
    memcpy(some_array2, some_array1, sizeof(some_array1));

    // char* new_values = "Bye  ";
    // memcpy(some_array1, new_values, 3);
    // memory_copy(some_array2, new_values, 3);
    // EXPECT_TRUE( 0 == memcmp( some_array1, some_array2, sizeof( some_array1 ) ) );

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
