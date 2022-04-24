#include <string.h>
#include <iostream>
#include "allocator.hpp"

#define MAX_TEXT_SIZE 1024 //BYTES

typedef struct Node{
    char data[MAX_TEXT_SIZE];
    struct Node *next;
} node, *node_ptr;

struct Stack{
    int size;
    node_ptr head;
};

//*****************************/
//========== Methods ==========/
//*****************************/

Stack* create();
void PUSH(Stack* stack, const char* text);
char* POP(Stack* stack, int *err_flag);
char* TOP(Stack* stack, int *err_flag);
bool isEmpty(Stack* stack);
void clear(Stack* stack);
void print_stack(Stack* stack);