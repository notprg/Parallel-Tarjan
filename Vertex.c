#include <stdio.h>
#include <stdlib.h>
#include "Vertex.h"

int vertexGreater(Vertex v1, Vertex v2) {
    return v1.value > v2.value;
}

int vertexLess(Vertex v1, Vertex v2) {
    return v1.value < v2.value;
}

bool sameVertex(Vertex *v1, Vertex *v2) {
    return (*v1).value == (*v2).value;
}

void vertexPrint(Vertex *v) {
    printf("%d -> ", v->value);
    for(int i = 0; i < *v->num_edges; i++) {
        printf("%d ", v->adj_list[i]);
    }
}

void addEdge(Vertex *v1, Vertex* v2) {
    v1->adj_list[*v1->num_edges] = v2->value;
    (*v1->num_edges)++;
}

Vertex* newVertex(int value) {
    Vertex *v = (Vertex *)malloc(sizeof(Vertex));
    v->value = value;
    v->num_edges = (int *)malloc(sizeof(int));
    *v->num_edges = 0;
    v->index = (int *)malloc(sizeof(int));
    *v->index = -1;
    v->onStack = (bool *)malloc(sizeof(bool));
    *v->onStack = false;
    v->low_link = (int *)malloc(sizeof(int));
    *v->low_link = -1;
    v->adj_list = (int *)malloc(10000 * sizeof(int));
    return v;
}
