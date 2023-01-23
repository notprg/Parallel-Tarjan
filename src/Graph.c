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
#include "../include/Graph.h"

Graph newGraph() {
    Graph g;
    g.num_vertex = 0;
    g.elements = malloc(100000 * sizeof(Vertex));
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
    #pragma omp parallel for
    for (int i = 0; i < g->num_vertex; i++)
    {
        if(g->elements[i]->value != -1) {
          vertexPrint(g->elements[i]);
          printf("\n");
        }
    }
}

void splitGraph(Graph *g, GraphSet *gs) {
    #pragma omp parallel for
    for(int i = 0; i < gs->num_graphs; i++) {
        for(int j = 0; j < g->num_vertex / gs->num_graphs; j++) {
          addVertex(&gs->graphs[i], g->elements[j + (i * (g->num_vertex / gs->num_graphs))]);
        }
    }
}

bool searchNode(Graph *g, Vertex *v) {
    bool result = false;
    #pragma omp parallel for
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
