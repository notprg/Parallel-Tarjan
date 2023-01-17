#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"

Graph newGraph() {
    Graph g;
    g.num_vertex = 0;
    g.elements = malloc(10000 * sizeof(Vertex));
    return g;
}

GraphSet newGraphSet() {
    GraphSet splitted_graphs;
    splitted_graphs.num_graphs = 0;
    for(int i = 0; i < 4; i++) {
        splitted_graphs.graphs[i] = newGraph();
    }
    return splitted_graphs;
}

void addVertex(Graph *g, Vertex *v) {
    g->elements[g->num_vertex] = *v;
    g->num_vertex++;
}

void printGraph(Graph *g) {
    for (int i = 0; i < g->num_vertex; i++)
    {
        vertexPrint(&g->elements[i]);
        printf("\n");
    }
}

void splitGraph(Graph *g, int n, GraphSet *gs) {
    gs->num_graphs = n;
    for(int i = 1; i < g->num_vertex; i++) {
        addVertex(&(gs->graphs[i % n]), &g->elements[i]);
    }
}
