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
#include <mpi.h>
#include <math.h>
#include "Vertex.h"
#include "Graph.h"
#include "tarjan.c"

void printSCC(Vertex **scc, int row, int column);

int main(int argc, char*argv[]) {

  static int scc_row = 0;
  static int scc_column = 0;
  MPI_Init(&argc, &argv);
  int size, rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size); 
  
  FILE *fp;
  fp = fopen("file.txt", "r");
  if(fp == NULL) {
      printf("Impossibile aprire il file!!!\n");
      exit(1);
  }

  int num_vertex, max_edges;
  fscanf(fp, "%d %d", &num_vertex, &max_edges);

  Vertex **vertices = (Vertex **) malloc(sizeof(Vertex *) * num_vertex);
  
  for(int j = 0; j < num_vertex; j++){
      vertices[j] = newVertex(j);
  }

  int vertice, vertice_collegato;
  while (fscanf(fp, "%d %d", &vertice, &vertice_collegato) == 2){
      addEdge(vertices[vertice], vertices[vertice_collegato]);
  }
  
  fclose(fp);

  Graph gr = newGraph();
  for(int i=0; i<num_vertex; i++){
      addVertex(&gr, vertices[i]);
  }
  
  //free(vertices);
  
  while(gr.num_vertex % size != 0) {
      addVertex(&gr, newVertex(-1));
  }
  
  Vertex **sccMatrix = (Vertex **)malloc(gr.num_vertex * sizeof(Vertex*)); 
  
  for(int i = 0; i < gr.num_vertex; i++) {
      sccMatrix[i] = (Vertex *)malloc(sizeof(Vertex) * gr.num_vertex);
  }
  
  Vertex *pof = newVertex(-1);
  for (int i = 0; i < gr.num_vertex; i++) {
      for(int j = 0; j < gr.num_vertex; j++) {
          sccMatrix[i][j] = *pof;
      }
  }
  
  int minigraph_num_vertex = (int)(gr.num_vertex / size);
  Vertex** v = (Vertex**)malloc(sizeof(Vertex*) * minigraph_num_vertex);
  
  for(int o = 0; o < minigraph_num_vertex; o++) {
      v[o] = newVertex(-1);
  } 
  
  Graph miniGraphs = newGraph();
  if(rank == 0){
      GraphSet gs = newGraphSet(size);
      splitGraph(&gr, &gs);  
      
      for(int i = 0; i < gs.num_graphs; i++){
          for(int j = 0; j < gs.graphs[i].num_vertex; j++){
              MPI_Send(&(gs.graphs[i].elements[j]->value), 1, MPI_INT, i, 0, MPI_COMM_WORLD); //OK
              MPI_Send(gs.graphs[i].elements[j]->num_edges, 1, MPI_INT, i, 3, MPI_COMM_WORLD); //OK 
              MPI_Send(gs.graphs[i].elements[j]->adj_list, sizeof(int) * (*gs.graphs[i].elements[j]->num_edges), MPI_BYTE, i, 1, MPI_COMM_WORLD);
          }
      }
  }
  for(int k = 0; k < minigraph_num_vertex; k++) {
    MPI_Recv(&v[k]->value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(v[k]->num_edges, 1, MPI_INT, 0, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(v[k]->adj_list, sizeof(int) * (*v[k]->num_edges), MPI_BYTE, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    addVertex(&miniGraphs, v[k]);
  }
  
  //free(v);
  Graph composedGraph = miniGraphs;
  static bool myGraphIsOk = false;
  
  for(int z = 0; z < log2(size); z++) { 
      int size_vect = (int)size / pow(2, z);
      int utility[size_vect];
      int stop = 0;
      for(int i = 0; i < size_vect; i++) {
          if(i < size_vect / 2)
              utility[i] = 0;
          else
              utility[i] = 1;
          stop = i;
      }
      for(int j = ++stop; j < size; j++) {
          utility[j] = -1;
      }
      
      if(utility[rank] == 1) 
      {
      
          sccMatrix = tarjan(composedGraph, sccMatrix, &scc_row, &scc_column);
          
          if(!myGraphIsOk) {
              for(int i = 0; i < scc_row; i++) {
                  for(int j = 0; j < gr.num_vertex; j++) {
                      if(sccMatrix[i][j].value != -1 && !searchNode(&composedGraph, &sccMatrix[i][j])) {
                          addVertex(&composedGraph, &sccMatrix[i][j]);
                      }
                      else {
                          break;
                      }
                  }
              }
          }
          //INVIO#pragma omp parallel for
          MPI_Send(&composedGraph.num_vertex, 1, MPI_INT, rank - (size_vect/2), 4, MPI_COMM_WORLD);
          
          for(int i = 0; i < composedGraph.num_vertex; i++) {
              MPI_Send(&(composedGraph.elements[i]->value), 1, MPI_INT, rank - (size_vect/2), 0, MPI_COMM_WORLD); //OK
              MPI_Send(composedGraph.elements[i]->num_edges, 1, MPI_INT, rank - (size_vect/2), 3, MPI_COMM_WORLD); //OK 
              MPI_Send(composedGraph.elements[i]->adj_list, sizeof(int) * (*composedGraph.elements[i]->num_edges), MPI_BYTE, rank - (size_vect/2), 10, MPI_COMM_WORLD);
          }
          //MUOIO
      }
      
      else if(utility[rank] == 0){
          int received_scc;          
          MPI_Recv(&received_scc, 1, MPI_INT, rank + (size_vect/2), 4, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          for(int i = 0; i < received_scc; i++) {
                  Vertex *rv = newVertex(-1);
                  MPI_Recv(&rv->value, 1, MPI_INT, rank + (size_vect/2), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                  MPI_Recv(rv->num_edges, 1, MPI_INT, rank + (size_vect/2), 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                  MPI_Recv(rv->adj_list, sizeof(int) * (*rv->num_edges), MPI_BYTE, rank + (size_vect/2), 10, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                  if(rv->value != -1) {
                      addVertex(&composedGraph, rv);
                  }
          }
          sccMatrix = tarjan(composedGraph, sccMatrix, &scc_row, &scc_column);
          myGraphIsOk = true;
          
          Vertex *pof = newVertex(-1);
          for (int i = 0; i < gr.num_vertex; i++) {
              for(int j = 0; j < gr.num_vertex; j++) {
                  sccMatrix[i][j] = *pof;
              }
          }
      }
  } 
    MPI_Barrier(MPI_COMM_WORLD);
    if(size == 1) {          
        Vertex **finalSCC = (Vertex **)malloc(gr.num_vertex * sizeof(Vertex*)); 

        for(int i = 0; i < gr.num_vertex; i++) {
            finalSCC[i] = (Vertex *)malloc(sizeof(Vertex) * gr.num_vertex);
        }
        
        Vertex *pof = newVertex(-1);
        for (int i = 0; i < gr.num_vertex; i++) {
            for(int j = 0; j < gr.num_vertex; j++) {
                finalSCC[i][j] = *pof;
            }
        }
        
        sccMatrix = tarjan(gr, sccMatrix, &scc_row, &scc_column); 
        //printSCC(sccMatrix, scc_row, composedGraph.num_vertex);
        free(sccMatrix);
    }
    else if(rank == 0) {
        
        while(composedGraph.num_vertex % size != 0) {
            addVertex(&composedGraph, newVertex(-1));
        }
                
        for(int i = 0; i < composedGraph.num_vertex; i++) {
            *composedGraph.elements[i]->index = -1;
            *composedGraph.elements[i]->low_link = -1;
            *composedGraph.elements[i]->onStack = false;
        }
        
        sccMatrix= tarjan(composedGraph, sccMatrix, &scc_row, &scc_column); 
        //printSCC(sccMatrix, scc_row, composedGraph.num_vertex);
        free(sccMatrix);
    }
    MPI_Finalize();
}

void printSCC(Vertex **scc, int row, int column) {
  for(int i = 0; i < row; i++) {
      printf("SCC n. %d: ", i);
      for(int j = 0; j < column; j++) {
          if(scc[i][j].value != -1 && scc[i][j].value != 0) {
              printf("%d ", scc[i][j].value); 
          }
          else {
              break;
          }
      }
      printf("\n");
  }
  printf("\n");
}
