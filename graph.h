#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <stdint.h>
#include "error.h"

typedef struct graph* pGraph;
typedef uint32_t graphNode;

rcode gCreateGraph(pGraph *);
	// Creates a graph
	// Returns 0 on success, or an appropriate error code if any error occurs

rcode gAddNode(pGraph, graphNode from, graphNode to);
	// Adds the node 'from'--->'to' to the graph 'pGraph'

rcode gFindShortestPath(pGraph, graphNode from, graphNode to/*, &solution*/);
	// Searches the graph 'pGraph' to find the shortest path from node 'from' to node 'to'

rcode gDestroyGraph(pGraph);
	// Frees all structs created by the graph, and then deletes him

#endif // _GRAPH_H_

