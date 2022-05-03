#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#define MAX_TEXT_SIZE 1024 //BYTES

typedef struct NodeDeque{
    char data[MAX_TEXT_SIZE];
    struct NodeDeque *next;
    struct NodeDeque *prev;
} deque_node, *deque_node_ptr;

typedef struct Deque{
    int size;
    deque_node_ptr head;
    deque_node_ptr tail;
}Deque;

//*****************************/
//========== Methods ==========/
//*****************************/

Deque* create_deque();
void ENQUEUE(Deque* queue, const char* text);
char* DEQUEUE_TAIL(Deque* queue, int *err_flag);  /*Original dequeue head*/
char* DEQUEUE(Deque* queue, int *err_flag);       /*Dequeue head*/
//char* TOP(Deque* queue);
bool isEmpty(Deque* stack);
void clear(Deque* stack);
int size(Deque* queue);
void print_queue(Deque* stack);