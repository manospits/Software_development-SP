#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <stdint.h>
#include "error.h"

#define BFS 0
#define BIDIRECTIONAL_BFS 1

typedef struct graph* pGraph;
typedef uint32_t graphNode;

pGraph gCreateGraph();
	// Creates a graph
	// Returns a pointer to the created graph on success, or NULL and sets error_val to an appropriate error code if any error occurs

rcode gDestroyGraph(pGraph *);
	// Frees all structs created by the graph, and then deletes him

rcode gAddEdge(pGraph, graphNode from, graphNode to);
	// Adds the node 'from'--->'to' to the graph 'pGraph'

int gFindShortestPath(pGraph, graphNode from, graphNode to, int type);
	// Searches the graph 'pGraph' to find the shortest path from node 'from' to node 'to'
	// Returns the length of the path found, or an appropriate error code (negative number) in case of error
	// 'type' is the type of search, defined above

int calculate_hashtable_size(pGraph g);

void gPrintGraph(pGraph);
	/*	prints the graph in format:
		Node <node number>
			 Outcoming: <node1> <node2> ... <node N>
			 Incoming: <node1> <node2> ... <node N>	*/

#endif // _GRAPH_H_
