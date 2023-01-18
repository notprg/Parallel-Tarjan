#ifndef VERTEX_H
#define VERTEX_H
#include <stdbool.h>

typedef struct Vertex {
    int value;
    int *index;
    bool *onStack;
    int *low_link;
    int *num_edges;
    int *adj_list;
} Vertex;

int vertexGreater(Vertex, Vertex);
int vertexLess(Vertex, Vertex);
bool sameVertex(Vertex*, Vertex*);
void vertexPrint(Vertex*);
void copyVertex(Vertex*, Vertex*);
void addEdge(Vertex*, Vertex*);
Vertex *newVertex(int);

#endif
