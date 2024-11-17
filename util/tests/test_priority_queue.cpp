#include <gtest/gtest.h>
using namespace std;

extern "C" {
    #include "../priority_queue.h"
    #include <sys/mman.h>
    #include <stdlib.h>
    #include <math.h>
}

TEST(priority_queue, insert) {

    priority_queue_t* pq = register_priority_queue();

    
    char a = 's';
    char b = 'u';
    char c = 'a';
    insert(pq, 5, &a);
    EXPECT_EQ(peek(pq)->priority, 5);
    insert(pq, 1, &b);
    EXPECT_EQ(peek(pq)->priority, 1);
    insert(pq, 3, &c);
    EXPECT_EQ(peek(pq)->priority, 1);
    insert(pq, 0, &c);
    EXPECT_EQ(peek(pq)->priority, 0);

    insert(pq, 0, &c);
    EXPECT_EQ(peek(pq)->priority, 0);

    insert(pq, 0, &c);
    EXPECT_EQ(peek(pq)->priority, 0);

    free(pq);

}

TEST(priority_queue, multipe_priority_queues) {

    priority_queue_t* pq = register_priority_queue();
    priority_queue_t* pq2 = register_priority_queue();

    

    cout << pq->size << endl;
    char a = 's';
    char b = 'u';
    char c = 'a';
    insert(pq, 5, &a);
    EXPECT_EQ(peek(pq)->priority, 5);
    insert(pq, 1, &b);
    EXPECT_EQ(peek(pq)->priority, 1);
    insert(pq, 3, &c);
    EXPECT_EQ(peek(pq)->priority, 1);
    insert(pq, 0, &c);
    EXPECT_EQ(peek(pq)->priority, 0);
    insert(pq, 6, &c);
    EXPECT_EQ(peek(pq)->priority, 0);
    insert(pq, 9, &c);
    EXPECT_EQ(peek(pq)->priority, 0);
    insert(pq, 0, &c);
    EXPECT_EQ(peek(pq)->priority, 0);

    insert(pq2, 10, &a);
    EXPECT_EQ(peek(pq2)->priority, 10);
    insert(pq2, 17, &b);
    EXPECT_EQ(peek(pq2)->priority, 10);
    insert(pq2, 3, &c);
    EXPECT_EQ(peek(pq2)->priority, 3);
    insert(pq2, 2, &c);
    EXPECT_EQ(peek(pq2)->priority, 2);

    free(pq);
    free(pq2);

}

TEST(priority_queue, poll) {

    priority_queue_t* pq = register_priority_queue();

    
    char a = 's';
    insert(pq, 5, &a);
    insert(pq, 6, &a);
    insert(pq, 3, &a);
    EXPECT_EQ(peek(pq)->priority, 3);


    insert(pq, 1, &a);
    EXPECT_EQ(peek(pq)->priority, 1);

    insert(pq, 8, &a);
    EXPECT_EQ(peek(pq)->priority, 1);
    
    EXPECT_EQ(poll(pq)->priority, 1);
    EXPECT_EQ(poll(pq)->priority, 3);
    EXPECT_EQ(poll(pq)->priority, 5);
    EXPECT_EQ(poll(pq)->priority, 6);
    EXPECT_EQ(poll(pq)->priority, 8);

    free(pq);

}




int main(int argc, char **argv) {

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}