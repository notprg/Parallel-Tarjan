#ifndef STACK_H
#define STACK_H
#include "Vertex.h"
#include "Array.h"

typedef struct {
    Array a;
} Stack;

Stack stackCreate();
void stackDestroy(Stack*);
void stackPush(Stack*, Vertex);
Vertex stackPop(Stack*);
Vertex stackTop(Stack*);
int stackIsEmpty(Stack*);

#endif