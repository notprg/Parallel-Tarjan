#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include "Vertex.h"
#include "Graph.h"
#include "tarjan.c"

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
  
  if(rank==0){
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
  
  tarjan(&miniGraphs, sccMatrix, &scc_row, &scc_column);

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
      if(utility[rank] == 1) {
          MPI_Send(&scc_row, 1, MPI_INT, rank - (size_vect/2), 4, MPI_COMM_WORLD);
          MPI_Send(&(miniGraphs.num_vertex), 1, MPI_INT, rank - (size_vect/2), 5, MPI_COMM_WORLD);
          for(int i = 0; i < scc_row; i++) {
              for(int j = 0; j < miniGraphs.num_vertex; j++) {
                  MPI_Send(&(sccMatrix[i][j].value), 1, MPI_INT, rank - (size_vect/2), 0, MPI_COMM_WORLD); //OK
                  MPI_Send(sccMatrix[i][j].num_edges, 1, MPI_INT, rank - (size_vect/2), 3, MPI_COMM_WORLD); //OK 
                  MPI_Send(sccMatrix[i][j].adj_list, sizeof(int) * (*sccMatrix[i][j].num_edges), MPI_BYTE, rank - (size_vect/2), 10, MPI_COMM_WORLD);
              }
          }
      }
      else if(utility[rank] == 0){
          bool flag = false;
          bool connectSCC = false;
          int received_scc;
          int received_num_vertex;
          int next = scc_row;
          int stop;
          MPI_Recv(&received_scc, 1, MPI_INT, rank + (size_vect/2), 4, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          MPI_Recv(&received_num_vertex, 1, MPI_INT, rank + (size_vect/2), 5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          Graph sccFound[scc_row + received_scc];
          for(int i = 0; i < scc_row; i++) { // RIGA PER RIGA CREO IL GRAFO
              sccFound[i] = newGraph();
              for(int j = 0; j < miniGraphs.num_vertex; j++) {
                  if(sccMatrix[i][j].value != -1) 
                      addVertex(&sccFound[i], &sccMatrix[i][j]);
              }
              stop = i;
          } 
          for(int j = ++stop; j < scc_row + received_scc; j++) {
              sccFound[j] = newGraph();
              for(int k = 0; k < miniGraphs.num_vertex; k++) {
                    addVertex(&sccFound[j], newVertex(-1));
              }
          }
          Vertex *rv = newVertex(-1);
          Graph sccReceived[received_scc];
          for(int i = 0; i < received_scc; i++) {
              sccReceived[i] = newGraph();
              for(int j = 0; j < received_num_vertex; j++) {
                  Vertex *rv = newVertex(-1);
                  MPI_Recv(&rv->value, 1, MPI_INT, rank + (size_vect/2), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                  MPI_Recv(rv->num_edges, 1, MPI_INT, rank + (size_vect/2), 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                  MPI_Recv(rv->adj_list, sizeof(int) * (*rv->num_edges), MPI_BYTE, rank + (size_vect/2), 10, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                  if(rv->value != -1) {
                      addVertex(&sccReceived[i], rv);
                  }
              }
          }
          for(int i = 0; i < received_scc; i++) { //PER TUTTE LE SCC (GRAFI) RICEVUTE
            for(int j = 0; j < scc_row; j++) { //PER TUTTE LE SCC (GRAFI) MIE
                for(int k = 0; k < sccFound[j].num_vertex; k++) { //PER TUTTI I VERTICI DELL'SCC DELLA RIGA DI SOPRA
                    for(int w = 0; w < *sccFound[j].elements[k]->num_edges; w++) {
                        for(int y = 0; y < sccReceived[i].num_vertex; y++) {
                            if(sccFound[j].elements[k]->adj_list[w] == sccReceived[i].elements[y]->value) { //IL VALORE TROVATO NEL MIO GRAFO = 
                                flag = true;
                            }
                        }
                    }
                }
                if(flag) {
                    for(int k = 0; k < sccReceived[i].num_vertex; k++) { //PER TUTTI I VERTICI DELL'SCC DELLA RIGA DI SOPRA
                        for(int w = 0; w < *sccReceived[i].elements[k]->num_edges; w++) {
                            for(int y = 0; y < sccFound[j].num_vertex; y++) {
                                if(sccReceived[i].elements[k]->adj_list[w] == sccFound[j].elements[y]->value) { //IL VALORE TROVATO NEL MIO GRAFO = 
                                    connectSCC = true;
                                }
                            }
                        }
                    } 
                }
                //flag = false;
                if(connectSCC) {
                    for(int x = 0; x < sccReceived[i].num_vertex; x++) {
                        addVertex(&sccFound[j], sccReceived[i].elements[x]);
                    }
                    //connectSCC = false;
                    //printf("Qua tutto ok\n");
                }
                else {
                    //AGGIUNGO L'SCC AL VETTORE DI SCC
                    for(int x = 0; x < sccReceived[i].num_vertex; x++) {
                        addVertex(&sccFound[next], sccReceived[i].elements[x]);
                    }
                    next++;
                    //printf("Qua segmentation fault\n");
                }
            }
        }
      }
    }
    if(rank == 0) {
        int conta = 0;
        for(int i = 0; i < scc_row; i++) {
            conta++;
        }
        printf("I've found %d SCC\n", conta);
    }
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
}
