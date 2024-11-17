

// an array of arrays
// each array is a separate priority queue
#include <stdbool.h>
#include <stddef.h>
#include "../common/math_util.h"

typedef struct priority_queue_node_t
{
    void* item;
    int priority;

}priority_queue_node_t;

typedef struct priority_queue_t {
    priority_queue_node_t** items;
    int size; 
}priority_queue_t;


void insert(priority_queue_t* priority_queue, int priority, void* item);

priority_queue_node_t* peek(priority_queue_t* priority_queue);

priority_queue_node_t* poll(priority_queue_t* priority_queue);

priority_queue_t* register_priority_queue();

char print_pq(int);