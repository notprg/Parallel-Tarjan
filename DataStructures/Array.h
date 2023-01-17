#ifndef ARRAY_H
#define ARRAY_H
#include "Vertex.h"

typedef struct {
    Vertex *items;
    int length;
    int size;
} Array;

Array arrayCreate(int);
void arrayDestroy(Array*);
void arrayResize(Array*, int);
void arrayAdd(Array*, Vertex);
Vertex arrayGet(Array*, int);
int arrayLength(Array*);
void arraySet(Array*, int, Vertex);
void arrayPrint(Array*);

#endif