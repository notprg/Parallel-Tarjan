#include <stdio.h>
#include <stdlib.h> 
#include <assert.h> 
#include "Array.h"

#define C_EXP 5
#define C_RED 10

Array arrayCreate(int length) {
    Array a;
    int size = length + C_EXP;
    a.items = malloc(size * sizeof(Vertex));
    assert(size == 0 || a.items != NULL);
    a.length = length;
    a.size = size;
    return a;
}

void arrayDestroy(Array *a) {
    free(a->items);
    a->items = NULL;
    a->length = a->size = 0;
}

void arrayResize(Array *a, int length) {
    if(length > a->size || length < a->size - C_RED) {
        int size = length + C_EXP;
        a->items = realloc(a->items, size * sizeof(Vertex));
        assert(size == 0 || a->items != NULL);
        a->size = size;
    }
    a->length = length;
}

void arrayAdd(Array *a, Vertex v) {
    arrayResize(a, a->length + 1);
    a->items[a->length - 1] = v;
}

Vertex arrayGet(Array *a, int pos) {
    return a->items[pos];
}

int arrayLength(Array *a) {
    return a->length;
}

void arraySet(Array *a, int pos, Vertex v) {
    if(pos >= a->length)
        arrayResize(a, pos + 1);
    a->items[pos] = v;
}

void arrayPrint(Array *a) {
    for(int i = 0; i < a->length; ++i) 
        printf("%d ", a->items[i].value);
    printf("\n");
}

