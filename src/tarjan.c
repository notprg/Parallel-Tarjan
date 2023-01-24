/*
# Course: High Performance Computing 2022/2023
# 
# Lecturer: Francesco Moscato fmoscato@unisa.it
#
# Group:
# Marcone Giuseppe 0622701896 g.marcone2@studenti.unisa.it               
# Pizzulo Rocco Gerardo 0622701990  r.pizzulo@studenti.unisa.it 
# Russo Luigi  0622702071  l.russo86@studenti.unisa.it
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
#include <omp.h>
#include "Graph.c"
#include "Vertex.c"

void printSCC(Vertex **scc, int row, int column);
int min(int a, int b);

/**
 * @brief This function looks for a SCC starting by a Vertex. If it will found an SCC, the low_link of each Vertex 
 *        in the SCC will be the same
 * @param g				Graph on which Tarjan's algorithm will be applied
 * @param u                             Starting point of the search
 * @param s			        Stack used to keep the visiting Vertices
 * @param stack_size		        Current index of the stack
 * @param sccMatrix                     Matrix that will contains the SCC found
 * @param scc_row                       Current row of the matrix
 * @param scc_column                    Current column of the matrix
**/
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

/**
 * @brief This function calls scc function on each Vertex of a graph. It also
 *        initialises a stack that will then be used in the search for SCC.
 * @param g				Graph on which Tarjan's algorithm will be applied
 * @param sccMatrix                     Matrix of SCC found, it will be initially empty
 * @param scc_row			Current number of SCC found
 * @param scc_column		        Current number of Vertex in the SCC found
 * @return sccMatrix                    Updated matrix of SCC found
**/

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

/**
 * @brief This function prints the SCCs found by Tarjan's algorithm in a graph.
 *        It runs the matrix row by row and prints out the values of the vertices
 *        of the SCC until it finds a value of 0, which indicates that the SCC is finished.
 * @param scc				Matrix of SCC, each row is a SCC
 * @param row				number of SCC found
 * @param column		        maximum number of Vertex for a SCC
**/
void printSCC(Vertex **scc, int row, int column) {
  #pragma omp parallel for
  for(int i = 0; i < row; i++) {
      printf("SCC n. %d: ", i);
      #pragma omp parallel for
      for(int j = 0; j < column; j++) {
          if(scc[i][j].value != -1 && scc[i][j].value != 0) {
              printf("%d ", scc[i][j].value); 
          }
          else {
              j = column;
          }
      }
      printf("\n");
  }
  printf("\n");
}

/**
 * @brief This function returns the minimum value of its parameters
 * @param a				First value to be compared
 * @param b				Second value to be compared
**/
int min(int a, int b) {
    return a < b ? a : b;
}
