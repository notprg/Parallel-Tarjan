#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "Vertex.h"
#include "Graph.h"
#include "tarjan.c"
#define ESCAPE -1

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
  for(int j = 0; j<num_vertex; j++){
    vertices[j] = newVertex(j);
  }

  int vertice, vertice_collegato;
  while (fscanf(fp, "%d %d", &vertice, &vertice_collegato) == 2){
    addEdge(vertices[vertice], vertices[vertice_collegato]);
  }

  Graph gr = newGraph();
  for(int i=0; i<num_vertex; i++){
    addVertex(&gr, vertices[i]);
  }
  
  while(gr.num_vertex % size != 0) {
    addVertex(&gr, newVertex(-1));
  }  
  //OK

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
  
  if(rank==0){
    GraphSet gs = newGraphSet(size);
    splitGraph(&gr, &gs);  
    
    //
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
  
  printGraph(&miniGraphs);
  printf("\n");
  tarjan(&miniGraphs, sccMatrix, &scc_row, &scc_column);
  for(int i = 0; i < scc_row; i++) {
    printf("SCC: ");
    for(int j = 0; j < gr.num_vertex; j++) {
      if(sccMatrix[i][j].value != -1) 
        printf("%d ", sccMatrix[i][j].value);
      else 
        break;
    }
    printf("\n");
  }

  MPI_Finalize(); 
} 
