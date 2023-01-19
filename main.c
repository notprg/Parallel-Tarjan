#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "Vertex.h"
#include "Graph.h"
#include "tarjan.c"
#define ESCAPE -1

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
  Vertex *ii = newVertex(9);
  Vertex *jj = newVertex(10);
  Vertex *kk = newVertex(11);
  Vertex *l = newVertex(12);
  Vertex *m = newVertex(13);
  Vertex *n = newVertex(14);
  Vertex *o = newVertex(15);
  Vertex *p = newVertex(16);
  Vertex *q = newVertex(17);
  Vertex *r = newVertex(18);
  Vertex *s = newVertex(19);
  Vertex *t = newVertex(20);
  Vertex *u = newVertex(21);
  Vertex *vv = newVertex(22);
  Vertex *w = newVertex(23);
  Vertex *x = newVertex(24);
  Vertex *y = newVertex(25);
  Vertex *z = newVertex(26);
  Vertex *aa = newVertex(27);
  addEdge(a, e);
  addEdge(b, e);
  addEdge(c, f);
  addEdge(d, g);
  addEdge(e, h);
  addEdge(f, ii);
  addEdge(g, kk);
  addEdge(h, jj);
  addEdge(ii, jj);
  addEdge(jj, kk);
  addEdge(m, jj);
  addEdge(kk, o);
  addEdge(kk, l);
  addEdge(o, m);
  addEdge(l, p);
  addEdge(o, n);
  addEdge(p, r);
  addEdge(r, o);
  addEdge(n, q);
  addEdge(q, r);
  addEdge(q, u);
  addEdge(r, s);
  addEdge(p, t);
  addEdge(t, vv);
  addEdge(vv, x);
  addEdge(s, w);
  addEdge(w, z);
  addEdge(w, aa);
  addEdge(u, x);

  Graph gr = newGraph();
  addVertex(&gr, a);
  addVertex(&gr, b);
  addVertex(&gr, c);
  addVertex(&gr, d);
  addVertex(&gr, e);
  addVertex(&gr, f);
  addVertex(&gr, g);
  addVertex(&gr, h);
  addVertex(&gr, ii);
  addVertex(&gr, jj);
  addVertex(&gr, kk);
  addVertex(&gr, l);
  addVertex(&gr, m);
  addVertex(&gr, n);
  addVertex(&gr, o);
  addVertex(&gr, p);
  addVertex(&gr, q);
  addVertex(&gr, r);
  addVertex(&gr, s);
  addVertex(&gr, t);
  addVertex(&gr, u);
  addVertex(&gr, vv);
  addVertex(&gr, w);
  addVertex(&gr, x);
  addVertex(&gr, y);
  addVertex(&gr, z);
  addVertex(&gr, aa);
  GraphSet gs = newGraphSet(size);
  splitGraph(&gr, &gs);
  
  /* ----------------------------- */
  
  sccMatrix = (int **)malloc(gr.num_vertex * sizeof(int *));
  for (int i = 0; i < gr.num_vertex; i++)
      sccMatrix[i] = (int *)malloc(gr.num_vertex * sizeof(int));
      
  for(int i = 0; i < gr.num_vertex; i++) {
    for(int j = 0; j < gr.num_vertex; j++) {
      sccMatrix[i][j] = ESCAPE;
    }
  }
  
  int minigraph_num_vertex = (int)(gr.num_vertex / size);
  Vertex* v[minigraph_num_vertex];
  for(int o = 0; o < minigraph_num_vertex; o++) {
    v[o] = newVertex(-1);
  }
  
  Graph miniGraphs = newGraph();
  
  if(rank==0){
    tarjan(&gr, sccMatrix, &scc_row, &scc_column);
    for(int i = 0; i < scc_row; i++) {
      printf("SCC: ");
      for(int j = 0; j < gr.num_vertex; j++) {
          if(sccMatrix[i][j] != ESCAPE)
            printf("%d ", sccMatrix[i][j]);
          else
            break;
      }
      printf("\n");
    }
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
  //printGraph(&miniGraphs);
  //printf("\n");
  //tarjan(&miniGraphs, sccMatrix, &scc_row, &scc_column);
  //printf("\n");

  /*for(int i = 0; i < scc_row; i++) {
      printf("SCC: ");
      for(int j = 0; j < scc_column; j++) {
          printf("%d\t", sccMatrix[i][j]);
      }
      printf("\n");
  }*/
  
  MPI_Finalize(); 
} 
