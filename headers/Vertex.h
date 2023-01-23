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


#ifndef VERTEX_H
#define VERTEX_H
#include <stdbool.h>

typedef struct Vertex {
    int value;
    int *index;
    bool *onStack;
    int *low_link;
    int *num_edges;
    int *adj_list;
} Vertex;

int vertexGreater(Vertex, Vertex);
int vertexLess(Vertex, Vertex);
bool sameVertex(Vertex*, Vertex*);
void vertexPrint(Vertex*);
void copyVertex(Vertex*, Vertex*);
void addEdge(Vertex*, Vertex*);
Vertex *newVertex(int);

#endif
