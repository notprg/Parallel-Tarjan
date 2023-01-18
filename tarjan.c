#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Graph.c"
#include "Vertex.c"

int min(int a, int b) {
    return a < b ? a : b;
}

void scc(Graph *g, Vertex *u, Vertex s[], int* stack_size) { 

    static int time = 0;
    *u->index = *u->low_link = ++time;
    s[(*stack_size)++] = *u;
    *u->onStack = true;

    for(int j = 0; j < *u->num_edges; j++) {
        Vertex *v = &u->adj_list[j];
        if(searchNode(g, v) == true) { 
            if(*v->index == -1) {
                scc(g, v, s, stack_size);
                *u->low_link = min(*u->low_link, *v->low_link);
            }
            else if(*v->onStack) {
                *u->low_link = min(*u->low_link, *v->index);
            }
        }
        else {
            printf("%d is not in this graph...\n", v->value);
        }
    }

    Vertex w; 
    if(*u->low_link == *u->index) {
        printf("SCC: ");
        while(w.value != u->value) {
            w = s[--(*stack_size)];
            printf("%d ", w.value);
            *w.onStack = false;
        }
        printf("\n");
    }
}

void tarjan(Graph *g) {
    
    Vertex* s = (Vertex*)malloc(sizeof(Vertex) * g->num_vertex);
    static int stack_size = 0;
    for(int i = 0; i < g->num_vertex; i++) {
        if(*g->elements[i].index == -1) {
            scc(g, &g->elements[i], s, &stack_size);
        }
    }
    
}
