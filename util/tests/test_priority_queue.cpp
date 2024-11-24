#include <gtest/gtest.h>
using namespace std;

extern "C" {
    #include "../priority_queue.h"
    #include <sys/mman.h>
    #include <stdlib.h>
    #include <math.h>
}

TEST(priority_queue, pq_insert) {

    priority_queue_t* pq = pq_register();

    
    char a = 's';
    char b = 'u';
    char c = 'a';
    pq_insert(pq, 5, &a);
    EXPECT_EQ(pq_peek(pq)->priority, 5);
    pq_insert(pq, 1, &b);
    EXPECT_EQ(pq_peek(pq)->priority, 1);
    pq_insert(pq, 3, &c);
    EXPECT_EQ(pq_peek(pq)->priority, 1);
    pq_insert(pq, 0, &c);
    EXPECT_EQ(pq_peek(pq)->priority, 0);

    pq_insert(pq, 0, &c);
    EXPECT_EQ(pq_peek(pq)->priority, 0);

    pq_insert(pq, 0, &c);
    EXPECT_EQ(pq_peek(pq)->priority, 0);

    free(pq);

}

TEST(priority_queue, multipe_priority_queues) {

    priority_queue_t* pq = pq_register();
    priority_queue_t* pq2 = pq_register();

    

    cout << pq->size << endl;
    char a = 's';
    char b = 'u';
    char c = 'a';
    pq_insert(pq, 5, &a);
    EXPECT_EQ(pq_peek(pq)->priority, 5);
    pq_insert(pq, 1, &b);
    EXPECT_EQ(pq_peek(pq)->priority, 1);
    pq_insert(pq, 3, &c);
    EXPECT_EQ(pq_peek(pq)->priority, 1);
    pq_insert(pq, 0, &c);
    EXPECT_EQ(pq_peek(pq)->priority, 0);
    pq_insert(pq, 6, &c);
    EXPECT_EQ(pq_peek(pq)->priority, 0);
    pq_insert(pq, 9, &c);
    EXPECT_EQ(pq_peek(pq)->priority, 0);
    pq_insert(pq, 0, &c);
    EXPECT_EQ(pq_peek(pq)->priority, 0);

    pq_insert(pq2, 10, &a);
    EXPECT_EQ(pq_peek(pq2)->priority, 10);
    pq_insert(pq2, 17, &b);
    EXPECT_EQ(pq_peek(pq2)->priority, 10);
    pq_insert(pq2, 3, &c);
    EXPECT_EQ(pq_peek(pq2)->priority, 3);
    pq_insert(pq2, 2, &c);
    EXPECT_EQ(pq_peek(pq2)->priority, 2);

    free(pq);
    free(pq2);

}

TEST(priority_queue, pq_poll) {

    priority_queue_t* pq = pq_register();

    
    char a = 's';
    pq_insert(pq, 5, &a);
    pq_insert(pq, 6, &a);
    pq_insert(pq, 3, &a);
    EXPECT_EQ(pq_peek(pq)->priority, 3);


    pq_insert(pq, 1, &a);
    EXPECT_EQ(pq_peek(pq)->priority, 1);

    pq_insert(pq, 8, &a);
    EXPECT_EQ(pq_peek(pq)->priority, 1);
    
    EXPECT_EQ(pq_poll(pq)->priority, 1);
    EXPECT_EQ(pq_poll(pq)->priority, 3);
    EXPECT_EQ(pq_poll(pq)->priority, 5);
    EXPECT_EQ(pq_poll(pq)->priority, 6);
    EXPECT_EQ(pq_poll(pq)->priority, 8);

    free(pq);

}




int main(int argc, char **argv) {

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
