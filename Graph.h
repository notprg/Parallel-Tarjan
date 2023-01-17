#ifndef GRAPH_H
#define GRAPH_H
#include "Vertex.h"

typedef struct {
    int num_vertex;
    Vertex *elements;
} Graph;

typedef struct {
    int num_graphs;
    Graph graphs[16];
} GraphSet;

Graph newGraph();
GraphSet newGraphSet();
void addVertex(Graph*, Vertex*);
void printGraph(Graph*);
void splitGraph(Graph*, int, GraphSet*);

#endif