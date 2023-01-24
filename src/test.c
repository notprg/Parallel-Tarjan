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

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../src/tarjan.c"

void printSCC(Vertex **scc, int row, int column) {
  for(int i = 0; i < row; i++) {
      printf("SCC n. %d: ", i);
      for(int j = 0; j < column; j++) {
          if(scc[i][j].value != -1 && scc[i][j].value != 0) {
              printf("%d ", scc[i][j].value); 
          }
          else {
              j = column;
          }
      }
      printf("\n");
  }
  printf("\n");
}

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

bool assert_matrix_equals(Vertex **expected, Vertex **obtained,  int rows, int cols) {
    int i, j, k, x;
    bool found;
    for(int i = 0; i < rows; i++) {
        found = true;
        for(int j = 0; j < rows; j++) {
            for(k = 0; k < cols; k++) {
                found = true;
                for(x = 0; x < cols; x++) {
                    if(!sameVertex(&expected[i][k], &obtained[j][x])) {
                        found = false;
                        break;
                    }
                }
            }
        }
        if(!found) {
            return false;
        }
    }
    return true;
}

int main(int argc, char **argv) {

    // -------------------------------- TEST 1 --------------------------------
    FILE *fp;
    fp = fopen("../test/test1.txt", "r");
    if(fp == NULL) {
        printf("Impossibile aprire il file!!!\n");
        exit(1);
    }

    int num_vertex, max_edges;
    fscanf(fp, "%d %d", &num_vertex, &max_edges);

    Vertex **vertices = (Vertex **) malloc(sizeof(Vertex *) * num_vertex);
    
    for(int j = 1; j <= num_vertex; j++){
        vertices[j] = newVertex(j);
    }

    int vertice, vertice_collegato;
    while (fscanf(fp, "%d %d", &vertice, &vertice_collegato) == 2){
        addEdge(vertices[vertice], vertices[vertice_collegato]);
    }
    
    fclose(fp);

    Graph gr = newGraph();
    for(int i=1; i<=num_vertex; i++){
        addVertex(&gr, vertices[i]);
    }

    
    Vertex **expected0 = (Vertex **)malloc(sizeof(Vertex *) * num_vertex);
    for(int i = 0; i < num_vertex; i++) {
        expected0[i] = (Vertex *)malloc(sizeof(Vertex) * num_vertex);
        for (int j = 0; j < num_vertex; j++) {
            expected0[i][j] = *newVertex(0);
        }
    }
    
    expected0[3][3].value = 5;
    expected0[2][5].value = 2;
    expected0[1][4].value = 6;
    expected0[1][1].value = 4;
    expected0[1][7].value = 8;
    expected0[1][8].value = 7;
    expected0[1][3].value = 9;
    expected0[1][2].value = 3;
    expected0[1][0].value = 10;
    expected0[1][5].value = 1;
    
    
    Vertex **obtained = (Vertex **)malloc(sizeof(Vertex *) * num_vertex);
  
    for(int i = 0; i < num_vertex; i++) {
        obtained[i] = (Vertex *)malloc(sizeof(Vertex) * num_vertex);
    }
    
    int static scc_row, scc_column = 0;
    obtained = tarjan(gr, obtained, &scc_row, &scc_column);
    
    bool result = assert_matrix_equals(expected0, obtained, gr.num_vertex, gr.num_vertex);
    
    if(result)
        printf("Test 1 passed succesfully!\n");
    else 
        printf("Test 1 failed!\n");
        
    for(int i = 0; i < num_vertex; i++) {
        free(obtained[i]);
        free(expected0[i]);
    }
    free(vertices);
    free(expected0);
    free(obtained);
        
    //-------------------------------- TEST 2 --------------------------------
    FILE *fp2;
    fp2 = fopen("../test/test2.txt", "r");
    if(fp2 == NULL) {
        printf("Impossibile aprire il file!!!\n");
        exit(1);
    }

    int num_vertex2, max_edges2;
    fscanf(fp2, "%d %d", &num_vertex2, &max_edges2);

    Vertex **vertices2 = (Vertex **) malloc(sizeof(Vertex *) * num_vertex2);
    
    for(int j = 1; j <= num_vertex2; j++){
        vertices2[j] = newVertex(j);
    }

    int vertice2, vertice_collegato2;
    while (fscanf(fp2, "%d %d", &vertice2, &vertice_collegato2) == 2){
        addEdge(vertices2[vertice2], vertices2[vertice_collegato2]);
    }
    
    fclose(fp2);

    Graph gr2 = newGraph();
    for(int i=1; i<=num_vertex2; i++){
        addVertex(&gr, vertices2[i]);
    }

    
    Vertex **expected = (Vertex **)malloc(sizeof(Vertex *) * num_vertex2);
    for(int i = 0; i < num_vertex2; i++) {
        expected[i] = (Vertex *)malloc(sizeof(Vertex) * num_vertex2);
        for (int j = 0; j < num_vertex2; j++) {
            expected[i][j] = *newVertex(0);
        }
    }
    
    expected[3][3].value = 29;
    expected[4][4].value = 17;
    expected[1][4].value = 38;
    expected[5][31].value = 42;
    expected[46][27].value = 26;
    expected[2][8].value = 16;
    expected[2][3].value = 5;
    expected[2][2].value = 8;
    expected[2][0].value = 6;
    expected[2][5].value = 25;
    expected[2][18].value = 21;
    expected[2][30].value = 48;
    expected[2][34].value = 34;
    expected[2][21].value = 40;
    expected[2][15].value = 45;
    expected[2][48].value = 12;
    expected[2][43].value = 10;
    expected[2][42].value = 44;
    expected[2][40].value = 23;
    expected[2][45].value = 39;
    expected[2][47].value = 43;
    expected[2][33].value = 49;
    expected[2][39].value = 35;
    expected[2][1].value = 46;
    expected[2][12].value = 30;
    expected[2][28].value = 20;
    expected[2][23].value = 13;
    expected[2][22].value = 9;
    expected[2][49].value = 18;
    expected[2][17].value = 4;
    expected[2][27].value = 14;
    expected[2][24].value = 7;
    expected[2][36].value = 31;
    expected[2][27].value = 33;
    expected[2][9].value = 22;  
    expected[2][19].value = 15;
    expected[2][6].value = 28;
    expected[2][32].value = 50;
    expected[2][10].value = 32;
    expected[2][11].value = 36;
    expected[2][7].value = 47;
    expected[2][26].value = 41;
    expected[2][46].value = 2;
    expected[2][41].value = 24;
    expected[2][12].value = 27;
    expected[2][13].value = 19;
    expected[2][14].value = 3;
    expected[2][25].value = 11;
    expected[2][35].value = 37;
        
    Vertex **obtained2 = (Vertex **)malloc(sizeof(Vertex *) * num_vertex2);
  
    for(int i = 0; i < num_vertex2; i++) {
        obtained2[i] = (Vertex *)malloc(sizeof(Vertex) * num_vertex2);
    }
    
    scc_row, scc_column = 0;
    obtained2 = tarjan(gr2, obtained2, &scc_row, &scc_column);
    
    bool result2 = assert_matrix_equals(expected, obtained2, gr2.num_vertex, gr2.num_vertex);
    
    if(result2)
        printf("Test 2 passed succesfully!\n");
    else 
        printf("Test 2 failed!\n");
        
    for(int i = 0; i < num_vertex2; i++) {
        free(obtained2[i]);
        free(expected[i]);
    }
    free(vertices2);
    free(expected);
    free(obtained2);
    
    
    
    return 0;
}

