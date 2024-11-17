

// an array of arrays
// each array is a separate priority queue
#include "priority_queue.h"
#include "math.h"


int queue_count = 0;
priority_queue_t** queue_list;


void swap(priority_queue_t* pq, priority_queue_node_t* node1, priority_queue_node_t* node2, int index_node1, int index_node2) {
    priority_queue_node_t* tmp = node1;
    pq->items[index_node1] = node2;
    pq->items[index_node2] = tmp;
    
}

void insert(priority_queue_t* priority_queue, int priority, void* item) {
    priority_queue_node_t* incoming_node = malloc(sizeof(priority_queue_node_t));

    incoming_node->item = item;
    incoming_node->priority = priority;

    int length = priority_queue->size;
    if (length == 0) {
        priority_queue->items[0] = incoming_node;
    } else {

        fizz_up(length, incoming_node, priority_queue);
    }
    priority_queue->size++;

}

void fizz_up(int index, priority_queue_node_t* incoming_node, priority_queue_t* priority_queue) {
    if (index == 0) {
        return;
    }

    int parent_index = index % 2 == 0 ? floor((index-1)/2) : floor(index/2);
    priority_queue_node_t* parent_node = priority_queue->items[parent_index];

    if (incoming_node->priority < parent_node->priority) {
        swap(priority_queue, incoming_node, parent_node, index, parent_index);
        fizz_up(parent_index, incoming_node, priority_queue);
    } else {
        priority_queue->items[index] = incoming_node;
    }
}

priority_queue_node_t* poll(priority_queue_t* priority_queue) {
    priority_queue_t* node = priority_queue->items[0];
    int length = priority_queue->size;
    if (priority_queue->size == 0) {
        return NULL;
    } else if (priority_queue->size > 1) {
        priority_queue->items[0] = priority_queue->items[length-1];
        sink(priority_queue, 0);
    } 
    priority_queue->size--;
    return node;
}

void sink(priority_queue_t* priority_queue, int index) {
    priority_queue_node_t* incoming_node = priority_queue->items[index];
    if (MAX(index*2+1, index*2+2) >= priority_queue->size ) {
        return;
    }
    printf("index %i\n", index);

    int l_idx = index*2+1;
    int r_idx = index*2+2;
    priority_queue_node_t* l_node = priority_queue->items[l_idx];
    priority_queue_node_t* r_node = priority_queue->items[r_idx];

    if (MIN(l_node->priority, r_node->priority) > incoming_node->priority) {
        return;
    }
    if (l_node->priority < r_node->priority) {
        swap(priority_queue, incoming_node, l_node, index, l_idx);
        sink(priority_queue, l_idx);
    } else {
        swap(priority_queue, incoming_node, r_node, index, r_idx);
        sink(priority_queue, r_idx);
    }
    

}

priority_queue_node_t* peek(priority_queue_t* priority_queue) {
    return priority_queue->items[0];
}

priority_queue_t* register_priority_queue() {
    priority_queue_t* queue = malloc(sizeof(priority_queue_t));
    queue->size = 0;
    queue->items = malloc(sizeof(priority_queue_node_t)*10);
    queue_list = realloc(queue_list, queue_count * sizeof(priority_queue_t));
    queue_list[queue_count++] = queue;
    return (void*) queue ;
}


char print_pq(int idx) {
}