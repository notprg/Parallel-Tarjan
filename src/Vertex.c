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
#include "../include/Vertex.h"

/**
* @brief This function verifies if v1 is greater than v2 
* @param v1   Vertex element
* @param v2   Vertex element
* @return 1 if v1 is greater than v2, 0 otherwise
*/
int vertexGreater(Vertex v1, Vertex v2) {
    return v1.value > v2.value;
}

/**
* @brief This function verifies if v1 is less than v2 
* @param v1   Vertex element
* @param v2   Vertex element
* @return 1 if v1 is less than v2, 0 otherwise
*/
int vertexLess(Vertex v1, Vertex v2) {
    return v1.value < v2.value;
}

/**
* @brief This function verifies if two vertex have the same value
* @param v1   pointer to a Vertex element
* @param v2   pointer to a Vertex element
* @return true if v1 and v2 have the same value, false otherwise
*/
bool sameVertex(Vertex *v1, Vertex *v2) {
    return (*v1).value == (*v2).value;
}

/**
* @brief This function print the value of the Vertex v and his adjacence list
* @param v   pointer to a Vertex element
*/
void vertexPrint(Vertex *v) {
    printf("%d -> ", v->value);
    #pragma omp parallel for
    for(int i = 0; i < *v->num_edges; i++) {
        printf("%d ", v->adj_list[i]);
    }
}

/**
* @brief This function add the Vertex v2 to the adjacence list of v1
* @param v1   pointer to a Vertex element
* @param v2   pointer to a Vertex element
*/
void addEdge(Vertex *v1, Vertex* v2) {
    v1->adj_list[*v1->num_edges] = v2->value;
    (*v1->num_edges)++;
}

/**
* @brief This function create a new Vertex with value as value, 0 as num_edges,
*            -1 as index, false as onStack, -1 as low_link and an empty adjacence list.
* @param value   Integer value for v->value
* @return v      New Vertex element
*/
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
    v->adj_list = (int *)malloc(500 * sizeof(int));
    return v;
}
