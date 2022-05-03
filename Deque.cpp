#include "Deque.hpp"

 Deque* create_deque() {
    Deque *queue = (Deque*)malloc(sizeof(Deque));
    if (!queue){
        return NULL;
    }
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;
    return queue;
}

// Enqueues to the head of the queue.
void ENQUEUE(Deque* queue, const char* text){
    deque_node_ptr n = (deque_node_ptr)malloc(sizeof(deque_node));
    if (!n){
        return;
    }
    strcpy(n->data, text);

    if(!queue->size){ /*Queue is empty*/
        n->next = NULL;
        n->prev = NULL;
        queue->head = n;
        queue->tail = n;
        queue->size++;
        return;
    }

    /*Set tail with the new inserted element to the dequeue*/
    queue->tail->prev = n;
    n->next = queue->tail;
    n->prev = NULL;
    queue->tail = n;
    queue->size++;
}


char* DEQUEUE(Deque* queue, int *err_flag){
    if(isEmpty(queue)){
        *err_flag = true;
        return NULL;
    }
    printf("Queue Size: %d\n", size(queue));
    deque_node_ptr head_prev = queue->head->prev;
    char* rm_node_str = queue->head->data;

    //printf("Queue Size: %s\n", head_prev->data);
    free(queue->head); // free dequeued resource
    queue->head = head_prev;
    if(queue->head) {queue->head->next = NULL;}
    else {queue->tail = NULL;}
    queue->size--;
    return rm_node_str;
}

char* DEQUEUE_TAIL(Deque* queue, int *err_flag){
    if(isEmpty(queue)){
        *err_flag = true;
        return NULL;
    }
    deque_node_ptr rm_node = queue->tail;
    char* rm_node_str = rm_node->data;
    deque_node_ptr tail_next = queue->tail->next;
    if (tail_next) {tail_next->prev = NULL;}
    else{queue->head = NULL;}
    queue->tail = tail_next;
    free(rm_node);
    return rm_node_str;
}

//char* TOP(Deque* queue);
//bool isEmpty(Deque* stack);
void clear(Deque* queue){

}

int size(Deque* queue){
    return queue->size;
}

bool isEmpty(Deque* queue){
    return queue->size == 0;
}

// Print function for debugging.
void print_queue(Deque* queue){
    deque_node_ptr curr = queue->head;
    printf("---- HEAD TO TAIL -----\n");
    while(curr){
        printf("%s ", curr->data);
        curr = curr->prev;
    }
    /* Now in reverse order*/
    curr = queue->tail; /*reset curr to tail*/
    printf("\n---- TAIL TO HEAD -----\n");
    while(curr){
        printf("%s ", curr->data);
        curr = curr->next;
    }

}