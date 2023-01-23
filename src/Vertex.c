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
    v->adj_list = (int *)malloc(50 * sizeof(int));
    return v;
}
