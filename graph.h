#ifndef _GRAPH_H_
#define _GRAPH_H_
#include "index.h"
#include "error.h"
#include "intlist.h"
#include "visited.h"
#include <stdint.h>

#define EDGE_EXISTS 666
#define BFS 0
#define BIDIRECTIONAL_BFS 1

#define NOTYPE -1
#define STATIC 0
#define DYNAMIC 1

typedef struct graph* pGraph;
typedef uint32_t graphNode;

pGraph gCreateGraph();
	// Creates a graph
	// Returns a pointer to the created graph on success, or NULL and sets error_val to an appropriate error code if any error occurs

rcode gDestroyGraph(pGraph *);
	// Frees all structs created by the graph, and then deletes him

rcode gAddEdge(pGraph, graphNode from, graphNode to);
	// Adds the node 'from'--->'to' to the graph 'pGraph'

rcode gAddEdge_t(pGraph, graphNode from, graphNode to,uint32_t version);
	// Adds the node 'from'--->'to' to the graph 'pGraph'

rcode create_indexes(pGraph ,int type);

int gFindShortestPath(pGraph, graphNode from, graphNode to, int type);
	// Searches the graph 'pGraph' to find the shortest path from node 'from' to node 'to'
	// Returns the length of the path found, or an appropriate error code (negative number) in case of error
	// 'type' is the type of search, defined above

int gFindShortestPath_t(pGraph g, graphNode from, graphNode to, phead *lists, pvis visited, uint32_t version);
    // Is the thread safe version of the above function,
    // Needs 2 intlists for CC/bidirectional searches ,version will be used only for dynamic graphs

int calculate_hashtable_size(pGraph g);

void gPrintGraph(pGraph);
	/*	prints the graph in format:
		Node <node number>
			 Outcoming: <node1> <node2> ... <node N>
			 Incoming: <node1> <node2> ... <node N>	*/

Index_ptr ret_inIndex(pGraph);
Index_ptr ret_outIndex(pGraph);
void print_ccid(pGraph,graphNode gnode);
void rebuild(pGraph);
#endif // _GRAPH_H_
