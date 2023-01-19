#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"

Graph newGraph() {
    Graph g;
    g.num_vertex = 0;
    g.elements = malloc(10000 * sizeof(Vertex));
    return g;
}

GraphSet newGraphSet(int num_graphs) {
    GraphSet splitted_graphs;
    splitted_graphs.num_graphs = num_graphs;
    for(int i = 0; i < splitted_graphs.num_graphs; i++) {
        splitted_graphs.graphs[i] = newGraph();
    }
    return splitted_graphs;
}

void addVertex(Graph *g, Vertex *v) {
    g->elements[g->num_vertex] = v;
    g->num_vertex++;
}

void printGraph(Graph *g) {
    for (int i = 0; i < g->num_vertex; i++)
    {
        if(g->elements[i]->value != -1) {
          vertexPrint(g->elements[i]);
          printf("\n");
        }
    }
}

void splitGraph(Graph *g, GraphSet *gs) {
    for(int i = 0; i < gs->num_graphs; i++) {
        for(int j = 0; j < g->num_vertex / gs->num_graphs; j++) {
          addVertex(&(gs->graphs[i]), g->elements[j + (i * (g->num_vertex / gs->num_graphs))]);
        }
        //addVertex(&(gs->graphs[i % gs->num_graphs]), g->elements[i]);
    }
}

bool searchNode(Graph *g, Vertex *v) {
    bool result = false;
    for(int i = 0; i < g->num_vertex; i++) {
        if(g->elements[i]->value == v->value)
          result = true;
    }
    return result;
}

Vertex* searchByValue(Graph *g, int value) {
    for(int i = 0; i < g->num_vertex; i++) {
        if(g->elements[i]->value == value)
          return g->elements[i];
    }
    return newVertex(-1);
}
