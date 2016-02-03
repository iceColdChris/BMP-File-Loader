#ifndef __HW3__LinkedList__
#define __HW3__LinkedList__

#include <stdlib.h>
#include <stdio.h>


typedef struct listNode {
    char* val;
    struct listNode * next;
} listNode;

void push(listNode *, char*);


#endif
