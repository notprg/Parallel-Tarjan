#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "Graph.c"
#include "Vertex.c"

int min(int a, int b) {
    return a < b ? a : b;
}

void scc(Graph *g, Vertex *u, Vertex s[], int* stack_size, int *sccMatrix[], int* scc_row, int* scc_column) { 

    static int time = 0;
    *u->index = *u->low_link = ++time;
    s[(*stack_size)++] = *u;
    *u->onStack = true;

    for(int j = 0; j < *u->num_edges; j++) {
        Vertex *v = searchByValue(g, u->adj_list[j]);
        if(v->value != -1) { 
            if(*v->index == -1) {
                scc(g, v, s, stack_size, sccMatrix, scc_row, scc_column);
                *u->low_link = min(*u->low_link, *v->low_link);
            }
            else if(*v->onStack) {
                *u->low_link = min(*u->low_link, *v->index);
            }
        }
    }

    Vertex w; 
    if(*u->low_link == *u->index) {
        (*scc_column) = 0;
        while(w.value != u->value) {
            w = s[--(*stack_size)];
            *w.onStack = false;
            sccMatrix[(*scc_row)][(*scc_column)] = w.value; 
            (*scc_column)++;
        }     
        (*scc_row)++;
    }
}

void tarjan(Graph *g, int** sccMatrix, int *scc_row, int *scc_column) {
    
  Vertex* s = (Vertex*)malloc(sizeof(Vertex) * g->num_vertex);
  static int stack_size = 0;
  for(int i = 0; i < g->num_vertex; i++) {
      if(*g->elements[i]->index == -1) {
          scc(g, g->elements[i], s, &stack_size, sccMatrix, scc_row, scc_column);
      }
  }   
}
