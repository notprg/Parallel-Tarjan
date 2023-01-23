#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include "Vertex.h"
#include "Graph.h"
#include "tarjan.c"

int fin = -1;
void printSCC(Vertex **scc, int row, int column);

int main(int argc, char*argv[]) {
  
  static int scc_row = 0;
  static int scc_column = 0;
  MPI_Init(&argc, &argv);
  int size, rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size); 
  //Graph init new version
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
  
  free(vertices);
  
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
  
  free(v);
  Graph composedGraph = newGraph();
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
      //MPI_Barrier(MPI_COMM_WORLD);
      if(utility[rank] == 1) { //SONO IL PROCESSO CHE DEVE INVIARE
          //COSTRUISCO IL GRAFO DELLE MIE SCC
          sccMatrix = tarjan(miniGraphs, sccMatrix, &scc_row, &scc_column);
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
          tarjan(miniGraphs, sccMatrix, &scc_row, &scc_column);
          int received_scc;          
          MPI_Recv(&received_scc, 1, MPI_INT, rank + (size_vect/2), 4, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          for(int i = 0; i < scc_row; i++) { 
              for(int j = 0; j < miniGraphs.num_vertex; j++) {
                  if(sccMatrix[i][j].value != -1 && !searchNode(&composedGraph, &sccMatrix[i][j])) {     
                      addVertex(&composedGraph, &sccMatrix[i][j]);
                  }
                  else {
                      j = miniGraphs.num_vertex;
                  }
              }
          }
          int vertex_value; 
          int vertex_adj_num;
          myGraphIsOk = true;
          
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
      }
      fin = z;
  }  
    //MPI_Barrier(MPI_COMM_WORLD);
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
        
        static int row = 0;
        static int column = 0;
        finalSCC = tarjan(gr, finalSCC, &row, &column); 
        printSCC(finalSCC, row, gr.num_vertex);
        free(finalSCC);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    if(rank == 0 && fin == log2(size) - 1) {
        while(composedGraph.num_vertex % size != 0) {
            addVertex(&composedGraph, newVertex(-1));
        }
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
        scc_row = scc_column = 0;
        finalSCC = tarjan(gr, finalSCC, &scc_row, &scc_column); 
        printSCC(finalSCC, scc_row, composedGraph.num_vertex);
        free(finalSCC);
    }
    MPI_Finalize();
}

void printSCC(Vertex **scc, int row, int column) {
  for(int i = 0; i < row; i++) {
      printf("SCC n. %d: ", i);
      for(int j = 0; j < column; j++) {
          if(scc[i][j].value != -1) {
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
