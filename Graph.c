#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"
/** 
*@brief This function creates a graph
*@return g        It returns an empty graph
*/
Graph newGraph() {
    Graph g;
    g.num_vertex = 0;
    g.elements = malloc(1000000 * sizeof(Vertex));
    return g;
}

/**
*@brief This function creates an empty GraphSet
*@return splitted_graphs    It returns a graphSet
*/
GraphSet newGraphSet(int num_graphs) {
    GraphSet splitted_graphs;
    splitted_graphs.num_graphs = num_graphs;
    for(int i = 0; i < splitted_graphs.num_graphs; i++) {
        splitted_graphs.graphs[i] = newGraph();
    }
    return splitted_graphs;
}

/**
*@brief This function add a vertex to a graph
*@param g                  poiter to graph
*@param v                  pointer to vertex
*/
void addVertex(Graph *g, Vertex *v) {
    g->elements[g->num_vertex] = v;
    g->num_vertex++;
}

/**
*@brief This function print a graph
*@param g                       pointer to graph
*/

void printGraph(Graph *g) {
    for (int i = 0; i < g->num_vertex; i++)
    {
        if(g->elements[i]->value != -1) {
          vertexPrint(g->elements[i]);
          printf("\n");
        }
    }
}

/**
*@brief This function split the graph and add the sub-graph to graphset
*@param g                       pointer to graph
*@param gs                      pointer to graphset
*/
void splitGraph(Graph *g, GraphSet *gs) {
    for(int i = 0; i < gs->num_graphs; i++) {
        for(int j = 0; j < g->num_vertex / gs->num_graphs; j++) {
          addVertex(&gs->graphs[i], g->elements[j + (i * (g->num_vertex / gs->num_graphs))]);
        }
    }
}


/**
*@brief This function verify if the graph contains a vertex
*@param g                       pointer to graph
*@param v                       pointer to vertex
*@return result                 it returns true if the graph contains the vertex, else it returns false   
*/
bool searchNode(Graph *g, Vertex *v) {
    bool result = false;
    for(int i = 0; i < g->num_vertex; i++) {
        if(g->elements[i]->value == v->value)
          result = true;
    }
    return result;
}

/**
*@brief This function searchs the value in the graph
*@param g                       pointer to graph
*@param value                   value to search
*@return result                 it returns the vertex searched if exists, else returns a default vertex with value -1 
*/
Vertex* searchByValue(Graph *g, int value) {
    for(int i = 0; i < g->num_vertex; i++) {
        if(g->elements[i]->value == value)
          return g->elements[i];
    }
    return newVertex(-1);
}

/**
*@brief This function compares two garphs
*@param a                       graph number one
*@param b                       graph number two
*@return result                 it returns true if the graphs are equal, else false
*/
bool graphEqual(Graph a, Graph b) {
    if(a.num_vertex != b.num_vertex) {
        printf("I numeri di elementi sono diversi\n%d - %d\n", a.num_vertex, b.num_vertex);
        return false;
    }
    bool result = false;
    for(int i = 0; i < a.num_vertex; i++) {
        for(int j = 0; j < b.num_vertex; j++) {
            if(a.elements[i]->value == b.elements[j]->value) {
                result = true;
                j = 0;
                break;
            }
        }
    }
    return result;
}
