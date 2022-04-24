#include <cstdio>
#include <stdio.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/types.h>

/*
 * For this implementation we used the following implementation,
 * published in the GitHub Gist and can be found in the following
 * link: https://gist.github.com/Shitaibin/8f8d80c45c9c4d23e4a2f264c49349a4
 */


typedef struct MemoryBlock{
    int size;              /* Block's size */
    int free;              /* If current block has allocated */
    MemoryBlock *next;     /* Next block */
    MemoryBlock *prev;     /* Previous block*/
    void* ptr;
    char data[1];
}memBlock, *memBlock_ptr;

void* myMalloc(size_t bytes);
void myFree(void* ptr);