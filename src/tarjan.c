/*
# Course: High Performance Computing 2022/2023
# 
# Lecturer: Francesco Moscato fmoscato@unisa.it
#
# Group:
# Marcone Giuseppe 0622701896 g.marcone2@studenti.unisa.it               
# Pizzulo Rocco Gerardo 0622701990  r.pizzulo@studenti.unisa.it 
# Russo Luigi  0622701  l.russo84@studenti.unisa.it
#
# This file is part of ParallelTarjan.
#
# ParallelTarjan is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# ParallelTarjan is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with ParallelTarjan.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "Graph.c"
#include "Vertex.c"

int min(int a, int b) {
    return a < b ? a : b;
}

void scc(Graph g, Vertex *u, Vertex s[], int* stack_size, Vertex *sccMatrix[], int* scc_row, int* scc_column) { 

    static int time = 0;
    *u->index = *u->low_link = ++time;
    s[(*stack_size)++] = *u;
    *u->onStack = true;

    for(int j = 0; j < *u->num_edges; j++) {
        Vertex *v = searchByValue(&g, u->adj_list[j]);
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
    if(*u->low_link == *u->index && u->value != -1) {
        (*scc_column) = 0;
        while(w.value != u->value) {
            w = s[--(*stack_size)];
            *w.onStack = false;
            sccMatrix[(*scc_row)][(*scc_column)] = w;
            (*scc_column)++;
        }   
        (*scc_row)++;
    }
}

Vertex **tarjan(Graph g, Vertex** sccMatrix, int *scc_row, int *scc_column) {
    
  Vertex* s = (Vertex*)malloc(sizeof(Vertex) * g.num_vertex);
  *scc_row = *scc_column = 0;
  static int stack_size = 0;
  for(int i = 0; i < g.num_vertex; i++) {
      if(*g.elements[i]->index == -1 && g.elements[i]->value != -1) {
          scc(g, g.elements[i], s, &stack_size, sccMatrix, scc_row, scc_column);
      }
  }   
  free(s);
  return sccMatrix;
}
