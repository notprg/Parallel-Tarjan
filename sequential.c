#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Vertex.h"
#include "Graph.h"
#include "tarjan.c"

int main(int argc, char*argv[]) {

  static int scc_row = 0;
  static int scc_column = 0;
  
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
  
  fclose(fp);
  
  Graph gr = newGraph();
  for(int i=0; i<num_vertex; i++){
      addVertex(&gr, vertices[i]);
  }
  
  free(vertices);
  
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
  
  sccMatrix = tarjan(&gr, sccMatrix, &scc_row, &scc_column);  
  return 0;
}
  
