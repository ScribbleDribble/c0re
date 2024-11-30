

// an array of arrays
// each array is a separate priority queue
#include "priority_queue.h"

int queue_count = 0;
priority_queue_t** queue_list;

void fizz_up(int index, priority_queue_node_t* incoming_node, priority_queue_t* priority_queue);

void pq_swap(priority_queue_t* pq, priority_queue_node_t* node1, priority_queue_node_t* node2, int index_node1, int index_node2) {
    priority_queue_node_t* tmp = node1;
    pq->items[index_node1] = node2;
    pq->items[index_node2] = tmp;
    
}

void pq_insert(priority_queue_t* priority_queue, int priority, void* item) {
    priority_queue_node_t* incoming_node = (priority_queue_node_t*)kmalloc(sizeof(priority_queue_node_t));

    // TODO realloc 

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

    int parent_index = index % 2 == 0 ? (index-1)/2 : index/2;
    priority_queue_node_t* parent_node = priority_queue->items[parent_index];

    if (incoming_node->priority < parent_node->priority) {
        pq_swap(priority_queue, incoming_node, parent_node, index, parent_index);
        fizz_up(parent_index, incoming_node, priority_queue);
    } else {
        priority_queue->items[index] = incoming_node;
    }
}

priority_queue_node_t* pq_poll(priority_queue_t* priority_queue) {
    priority_queue_node_t* node = priority_queue->items[0];
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

    int l_idx = index*2+1;
    int r_idx = index*2+2;
    priority_queue_node_t* l_node = priority_queue->items[l_idx];
    priority_queue_node_t* r_node = priority_queue->items[r_idx];

    if (MIN(l_node->priority, r_node->priority) > incoming_node->priority) {
        return;
    }
    if (l_node->priority < r_node->priority) {
        pq_swap(priority_queue, incoming_node, l_node, index, l_idx);
        sink(priority_queue, l_idx);
    } else {
        pq_swap(priority_queue, incoming_node, r_node, index, r_idx);
        sink(priority_queue, r_idx);
    }
}

priority_queue_node_t* pq_peek(priority_queue_t* priority_queue) {
    return priority_queue->items[0];
}

priority_queue_t* pq_register() {
    if (queue_count == 0) {
        queue_list = kmalloc(sizeof(priority_queue_t));
    } else {
        queue_list = (priority_queue_t**) krealloc(queue_list, queue_count * sizeof(priority_queue_t));
    }
    priority_queue_t* queue = (priority_queue_t*) kmalloc(sizeof(priority_queue_t));
    queue->size = 0;
    queue->items = (priority_queue_node_t*) kmalloc(sizeof(priority_queue_node_t)*10);
    memory_set(queue->items, NULL, sizeof(priority_queue_node_t)*10);
    queue_list[queue_count++] = queue;

    return queue ;
}

void pq_close(priority_queue_t* priority_queue) {
    for (int i = 0; i < priority_queue->size; i++) {
        free(priority_queue->items[i]);
    }
    free(priority_queue);
}