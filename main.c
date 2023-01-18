#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "Vertex.h"
#include "Graph.h"
#include "tarjan.c"

int main(int argc, char*argv[]) {

      // DIVIDERE IL GRAFO IN SOTTOGRAFI CON VERTICI CONTINUI (11,12,13,14,...) E MAPPARLI ALL'INDICE
      // DEL VETTORE SALVANDO UN DISPLACEMENT (in questo caso il displacement sarà 10 perché il nodo 0 è il pof)
      MPI_Init(&argc, &argv);
      int size, rank;
      MPI_Comm_rank(MPI_COMM_WORLD, &rank);
      MPI_Comm_size(MPI_COMM_WORLD, &size); 
      

      //Graph init
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
      
      int minigraph_num_vertex = (int)(gr.num_vertex / size);
      Vertex* v[minigraph_num_vertex];
      for(int o = 0; o < minigraph_num_vertex; o++) {
        v[o] = newVertex(-1);
      }
      
      Graph miniGraphs = newGraph();
      
      if(rank==0){
        GraphSet gs = newGraphSet(size);
        splitGraph(&gr, &gs);

        for(int i = 0; i < gs.num_graphs; i++){
          for(int j = 0; j < gs.graphs[i].num_vertex; j++){
            MPI_Send(&(gs.graphs[i].elements[j].value), 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(gs.graphs[i].elements[j].index, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(gs.graphs[i].elements[j].low_link, 1, MPI_INT, i, 2, MPI_COMM_WORLD);
            MPI_Send(gs.graphs[i].elements[j].num_edges, 1, MPI_INT, i, 3, MPI_COMM_WORLD);
            MPI_Send(gs.graphs[i].elements[j].onStack, 1, MPI_C_BOOL, i, 4, MPI_COMM_WORLD);
            MPI_Send(gs.graphs[i].elements[j].adj_list, sizeof(Vertex)*(*b->num_edges), MPI_BYTE, i, 5, MPI_COMM_WORLD);
          }
        }
      }
      for(int k = 0; k < minigraph_num_vertex; k++) {
        MPI_Recv(&v[k]->value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(v[k]->index, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(v[k]->low_link, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(v[k]->num_edges, 1, MPI_INT, 0, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(v[k]->onStack, 1, MPI_C_BOOL, 0, 4, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(v[k]->adj_list, 5*sizeof(Vertex)*(*v[k]->num_edges), MPI_BYTE, 0, 5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        addVertex(&miniGraphs, v[k]);
      }
      printGraph(&miniGraphs);
      printf("\n");
      //tarjan(&miniGraphs);
      printf("\n");
      MPI_Finalize(); 
} 


//VERIFICARE COSTRUZIONE G1
