#include "allocator.hpp"

#define BLOCK_SIZE 40
memBlock_ptr head_block = NULL;

/* Find an suitable block using first fit */
memBlock_ptr find_block(memBlock_ptr *last, size_t size) {
    memBlock_ptr b = head_block;
    while (b && !(b->free && b->size >= size)) {
        *last = b;
        b = b->next;
    }
    return b;
}

/* Add a new block at the end of heap */
/* return NULL if things go wrong */
memBlock_ptr extend_heap(memBlock_ptr last, size_t s) {
    memBlock_ptr b;
    b = (memBlock_ptr)sbrk(0);
    if (sbrk(BLOCK_SIZE + s) == (void*) -1)
        return NULL;
    b->size = s;
    b->next = NULL;
    b->prev = last;
    b->ptr = b->data;
    if (last)
        last->next = b;
    b->free = 0;
    return b;
}

/* Split block according to size. */
/* The b block must be exist.     */
void split_block(memBlock_ptr b, size_t s) {
    memBlock_ptr new_block;
    new_block = (memBlock_ptr)(b->data + s);
    /* insert new_block between b and b->next */
    new_block->next = b->next;
    new_block->prev = b;
    b->next = new_block;
    if (new_block->next)
        new_block->next->prev = new_block;
    /* update meta of new_block */
    new_block->size = b->size - s - BLOCK_SIZE;
    new_block->ptr = b->data;
    new_block->free = 1;
    /* update meta of b */
    b->size = s;
}

/* Align size */
size_t align8(size_t s) {
    /* s % 8 */
    if ((s & 0x7) == 0)
        return s;
    return ((s >> 3) + 1) << 3;
}


void* myMalloc(size_t size)
{
    memBlock_ptr b, last;
    size_t s;

    /* align */
    s = align8(size);
    if (head_block) {
        /* find suitable block */
        last = head_block;
        b = find_block(&last, s);
        if (b) {
            /* too big, split */
            if ((b->size - s) >= (BLOCK_SIZE + 8)) {
                split_block(b, s);
            }
            /* */
            b->free = 0;
        }
        else {
            /* no suitable block, create a new one */
            b = extend_heap(last, s);
            if (!b) return NULL;
        }
    }
    else {
        b = extend_heap(NULL, s);
        if (!b) return NULL;
        head_block = b;
    }
    return b->data;
}


void* calloc(size_t n, size_t size) {
    size_t* new_block;
    size_t s8, i;
    new_block = (size_t*)myMalloc(n * size);
    if (new_block) {
        /* the number os groups */
        s8 = align8(n * size) >> 3;
        for (i = 0; i < s8; i++)
            new_block[i] = 0;
    }
    return new_block;
}

memBlock_ptr get_block(void *p) {
    char *tmp;
    tmp = (char *)p;
    return (memBlock_ptr)(p = tmp -= BLOCK_SIZE);
}

int valid_addr(void *p) {
    if (head_block) {
        if (p > head_block && p <sbrk(0)) {
            return (p == (get_block(p))->ptr);
        }
    }
    /* No block */
    return 0;
}

memBlock_ptr fusion(memBlock_ptr b) {
    if (b->next && b->next->free) {
        b->size += BLOCK_SIZE + b->next->size;
        b->next = b->next->next;
        if (b->next)
            b->next->prev = b;
    }
    return b;
}

void myFree(void* ptr) {
    memBlock_ptr b;
    if (valid_addr(ptr)) {
        b = get_block(ptr);
        b->free = 1;
        /* fusion with previous if possible */
        if (b->prev && b->prev->free)
            b = fusion(b->prev);
        /* then fusion with next */
        if (b->next)
            fusion(b);
        else {
            /* reach the break, free the end of the heap */
            if (b->prev)
                b->prev->prev = NULL;
            else
                /* NO more block */
                head_block = NULL;
                brk(b);
        }
    }
}