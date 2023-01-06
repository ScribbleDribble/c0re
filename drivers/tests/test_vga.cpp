#include <gtest/gtest.h>

extern "C" {
    #include "../vga.h"
    #include "../../kernel/string.h"
    #include <sys/mman.h>
}

// test kprint_char, kprint kputs and scrolling

TEST(KPutsTests, TestMemoryCopy) {
    // request OS to map virtual address (VIDEO_ADDRESS) in physical memor
    int prot = PROT_READ | PROT_WRITE;
    int flags = MAP_PRIVATE | MAP_ANONYMOUS;
    mmap((void*) VIDEO_ADDRESS, MAX_HEIGHT*MAX_WIDTH*4, prot, flags, -1, 0);

    // then print stuff to screen
    char* video_memory = (char*)VIDEO_ADDRESS;
    memset(video_memory, '0', MAX_HEIGHT*MAX_WIDTH*CHAR_SIZE);
    // for (int i = 0; i < MAX_HEIGHT; i++) {
    //     char buf[16];
    //     int_to_str(i % 10, buf, 16);
    //     kputs(buf);    
    // }
    
    char* cur_addr = video_memory;
    int row = 0;
    int col = 0;
    for (int i = 0; i < MAX_HEIGHT*MAX_WIDTH; i++) {
        if (col == MAX_WIDTH) {
            col = 0;
            row += 1;
            EXPECT_EQ(*cur_addr, (char) '0' + row % 10) << row << " " << col;
        } else {
            EXPECT_EQ(*cur_addr, '0') << row << " " << col;
        }

        cur_addr += 2;
        col += 1;
    }
    
}


int main(int argc, char **argv) {

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
