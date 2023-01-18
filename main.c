#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "Vertex.h"
#include "Graph.h"
#include "tarjan.c"

int main(int argc, char*argv[]) {

  int **sccMatrix;
  static int scc_row = 0;
  static int scc_column = 0;

  MPI_Init(&argc, &argv);
  int size, rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size); 
  
  //Graph initialization
  /* ----------------------------- */
  Vertex *a = newVertex(1);
  Vertex *b = newVertex(2);
  Vertex *c = newVertex(3);
  Vertex *d = newVertex(4);
  Vertex *e = newVertex(5);
  Vertex *f = newVertex(6);
  Vertex *g = newVertex(7);
  Vertex *h = newVertex(8);
  addEdge(a, b);
  addEdge(b, c);
  addEdge(b, f);
  addEdge(b, e);
  addEdge(c, g);
  addEdge(c, d);
  addEdge(d, c);
  addEdge(d, h);
  addEdge(e, a);
  addEdge(e, f);
  addEdge(f, g);
  addEdge(g, f);
  addEdge(h, g);
  addEdge(h, d);

  Graph gr = newGraph();
  addVertex(&gr, a);
  addVertex(&gr, b);
  addVertex(&gr, c);
  addVertex(&gr, d);
  addVertex(&gr, e);
  addVertex(&gr, f);
  addVertex(&gr, g);
  addVertex(&gr, h);
  GraphSet gs = newGraphSet(size);
  splitGraph(&gr, &gs);
  
  /* ----------------------------- */
  
  sccMatrix = (int **)malloc(gr.num_vertex * sizeof(int *));
  for (int i = 0; i < gr.num_vertex; i++)
      sccMatrix[i] = (int *)malloc(gr.num_vertex * sizeof(int));
  
  int minigraph_num_vertex = (int)(gr.num_vertex / size);
  Vertex* v[minigraph_num_vertex];
  for(int o = 0; o < minigraph_num_vertex; o++) {
    v[o] = newVertex(-1);
  }
  
  Graph miniGraphs = newGraph();
  
  if(rank==0){
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
  printf("\n");

  for(int i = 0; i < scc_row; i++) {
      printf("SCC: ");
      for(int j = 0; j < scc_column; j++) {
          printf("%d\t", sccMatrix[i][j]);
      }
      printf("\n");
  }
  
  MPI_Finalize(); 
} 
