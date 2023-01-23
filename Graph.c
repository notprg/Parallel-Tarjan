#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"

Graph newGraph() {
    Graph g;
    g.num_vertex = 0;
    g.elements = malloc(1000000 * sizeof(Vertex));
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
          addVertex(&gs->graphs[i], g->elements[j + (i * (g->num_vertex / gs->num_graphs))]);
        }
    }
}

/*graphSet dividi(Grafo *grafo, int nDivisioni){
    graphSet aggiunta = newGraphSet();
    aggiunta.numGraph = nDivisioni;
    for (int i = 0; i < grafo->num_element; i++){
        addVertex(&aggiunta.grafi[i % nDivisioni], arrayGetV(&grafo->element, i));
    }
    arrayDestroyV(&grafo->element);
    
    return aggiunta;
}*/

/*void splitGraph(Graph *g, GraphSet *gs) {
      for(int j = 0; j < g->num_vertex; j++) {
        addVertex(&gs->graphs[j % gs->num_graphs], g->elements[j]);
      }
}*/

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

bool graphEqual(Graph a, Graph b) {
    if(a.num_vertex != b.num_vertex) {
        printf("I numeri di elementi sono diversi\n%d - %d\n", a.num_vertex, b.num_vertex);
        return false;
    }
    bool result = false;
    for(int i = 0; i < a.num_vertex; i++) {
        for(int j = 0; j < b.num_vertex; j++) {
            if(a.elements[i]->value == b.elements[j]->value) {
                result = true;
                j = 0;
                break;
            }
        }
    }
    return result;
}
