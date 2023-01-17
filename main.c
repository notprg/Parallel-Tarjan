#include <stdio.h>
#include <stdlib.h>
#include "Vertex.h"
#include "Graph.h"
#include "tarjan.c"

int main() {

    // DIVIDERE IL GRAFO IN SOTTOGRAFI CON VERTICI CONTINUI (11,12,13,14,...) E MAPPARLI ALL'INDICE
    // DEL VETTORE SALVANDO UN DISPLACEMENT (in questo caso il displacement sarà 10 perché il nodo 0 è il pof)

    Vertex *a = newVertex(1);
    Vertex *b = newVertex(2);
    Vertex *c = newVertex(3);
    Vertex *d = newVertex(4);
    Vertex *e = newVertex(5);
    Vertex *f = newVertex(6);
    Vertex *g = newVertex(7);
    Vertex *h = newVertex(8);
    addEdge(a, b);
    addEdge(b, c);
    addEdge(b, f);
    addEdge(b, e);
    addEdge(c, g);
    addEdge(c, d);
    addEdge(d, c);
    addEdge(d, h);
    addEdge(e, a);
    addEdge(e, f);
    addEdge(f, g);
    addEdge(g, f);
    addEdge(h, g);
    addEdge(h, d);
    
    Graph gr = newGraph();
    addVertex(&gr, a);
    addVertex(&gr, b);
    addVertex(&gr, c);
    addVertex(&gr, d);
    addVertex(&gr, e);
    addVertex(&gr, f);
    addVertex(&gr, g);
    addVertex(&gr, h);
    printGraph(&gr);
    printf("\n\n");
    tarjan(&gr); 
    return 0;
} 