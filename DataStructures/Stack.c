#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Stack.h"

Stack stackCreate() {
    Stack s;
    s.a = arrayCreate(0);
    return s;
}

void stackDestroy(Stack *s) {
    arrayDestroy(&s->a);
}

void stackPush(Stack *s, Vertex v) {
    arrayResize(&s->a, s->a.length + 1);
    s->a.items[s->a.length - 1] = v;
}

Vertex stackPop(Stack *s) {
    Vertex v = s->a.items[s->a.length - 1];
    arrayResize(&s->a, s->a.length - 1);
    return v;
}

Vertex stackTop(Stack *s) {
    return s->a.items[s->a.length - 1];
}

int stackIsEmpty(Stack *s) {
    return s->a.length == 0;
}