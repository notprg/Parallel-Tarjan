#include<stdio.h>
#include<stdlib.h>
#include<time.h>

//#define MAX_VERTICES 30
//#define MAX_EDGES 10

typedef unsigned char vertex; //da capire perchè viene usato un unsigned char

int main(int argc, char* argv[]){

    FILE *fp;
    fp = fopen("file.txt", "w");
    if(fp == NULL) {
        printf("Impossibile aprire il file!!!\n");
        exit(1);
    }
    
    //GENERAZIONE N. VERTICI E N.MAX ARCHI
    //La funzione time(NULL) ritorna il valore in secondi  tra il momento di avvio del programma e il 1/1/1970.
    //L'argomento di srand è il seme dall'algoritmo del generatore di numeri pseudo-casuali.
    //L’istruzione srand(time(NULL)) inizializza la generazione di numeri casuali che verrà usato dalla funzione rand() in modo che a ogni esecuzione generi numeri diversi.
    /*numero di nodi in un grafo*/
    int MAX_VERTICES = atoi(argv[1]);
    int MAX_EDGES = atoi(argv[2]);

    srand(time(NULL));
    int numeroDiVertici = rand() % MAX_VERTICES;
    //int numeroDiVertici = MAX_VERTICES;
    
    /*numero massimo di archi che un nodo può avere*/
    srand(time(NULL));
    int maxNumberOfEdge = rand() % MAX_EDGES;
    //int maxNumberOfEdge = MAX_EDGES;

    // Il grafo deve avere almeno un nodo
    if(numeroDiVertici == 0)
        numeroDiVertici++;
    

    vertex ***graph;
    printf("Numero totale di vertici = %d, Massimo numero di archi = %d\n", numeroDiVertici,maxNumberOfEdge);
    fprintf(fp, "%d %d", numeroDiVertici,maxNumberOfEdge);

    /*genero un array dinamico*/


    if((graph = (vertex ***) malloc(sizeof(vertex **) * numeroDiVertici)) == NULL){
        printf("Non riesco ad allocare la memoria per il grafo\n");
        exit(1);
    }

    int vertexCount = 0;    
    int edgeCount = 0;

    for(vertexCount=0; vertexCount<numeroDiVertici; vertexCount++){
        if((graph[vertexCount] = (vertex **) malloc(sizeof(vertex *) * maxNumberOfEdge)) == NULL){
            printf("Non riesco ad allocare la memoria per gli archi\n");
            exit(1);
        }
        for(edgeCount=0; edgeCount<maxNumberOfEdge; edgeCount++){
            if((graph[vertexCount][edgeCount] = (vertex *) malloc(sizeof(vertex))) == NULL){
                printf("Non riesco ad allocare la memoria per i vertici\n");
                exit(1);
            }
        }
    }

    fflush(stdin);
    /*inizio a collegare il grafo. Tutti i vertici hanno bisogno di avere un numero di link*/
    vertexCount = 0; edgeCount = 0;
    for(vertexCount = 0; vertexCount<numeroDiVertici; vertexCount++){
        //printf("%d:\t", vertexCount);
        //fprintf(fp, "%d ", vertexCount);
        for(edgeCount=0; edgeCount<maxNumberOfEdge; edgeCount++){
            if(rand()%2 == 1){/*collego i vertici*/
                int verticeCollegato = rand() % numeroDiVertici;
                graph[vertexCount][edgeCount] = graph[verticeCollegato];
                printf("\n%d %d ", vertexCount, verticeCollegato);
                fprintf(fp, "\n%d %d ", vertexCount, verticeCollegato);
            }
            else{/*creo il link NULL*/
                graph[vertexCount][edgeCount] = NULL;
            }
        }
        //printf("\n");
        //fprintf(fp, "\n");
    }
    return 1;
}
