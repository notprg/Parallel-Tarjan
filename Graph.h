#ifndef GRAPH_H
#define GRAPH_H
#include "Vertex.h"

typedef struct {
    int num_vertex;
    Vertex **elements;
} Graph;

typedef struct {
    int num_graphs;
    Graph graphs[16];
} GraphSet;

Graph newGraph();
GraphSet newGraphSet(int);
void addVertex(Graph*, Vertex*);
void printGraph(Graph*);
void splitGraph(Graph*, GraphSet*);
Vertex* searchByValue(Graph *, int);
bool searchNode(Graph*, Vertex*);
bool graphEqual(Graph, Graph);
#endif
