#include <stdlib.h>
#include "graph.h"
#include "buffer.h"
#include "index.h"
#include "hash.h"
#include "intlist.h"

typedef struct graph
{
	Index_ptr inIndex;
	Index_ptr outIndex;
}graph;

pGraph gCreateGraph()
{
	pGraph g;
	if ((g = malloc(sizeof(graph))) == NULL)
	{
		error_val = GRAPH_CREATION_BASIC_STRUCT_MALLOC_FAIL;
		return NULL;
	}
	if ((g->inIndex = createNodeIndex()) == NULL)
	{
		free(g);
		error_val = GRAPH_CREATION_INDEX_MALLOC_FAIL;
		return NULL;
	}
	if ((g->outIndex = createNodeIndex()) == NULL)
	{
		destroyNodeIndex(g->inIndex);
		free(g);
		error_val = GRAPH_CREATION_INDEX_MALLOC_FAIL;
		return NULL;
	}
	return g;
}

rcode gAddNode(pGraph g, graphNode from, graphNode to)
{
	if (g == NULL)
	{
		error_val = GRAPH_NULL_POINTER_PROVIDED;
		return GRAPH_NULL_POINTER_PROVIDED;
	}
	if (from == to)
	{
		error_val = OK_SUCCESS;
		return OK_SUCCESS;
	}
	graphNode max = from;
	if (to > max) max = to;
	rcode return_value;
	// check if nodes exist (insert nodes checks, and adds them if they don't exist)
	// (insert the max of the two nodes, so in case it doesn't exist indexes are updated to include him)
	return_value = insertNode(g->outIndex, max);
	if (return_value) return return_value;
	return_value = insertNode(g->inIndex, max);
	if (return_value) return return_value;
	// check if edge exists (no need to check both indexes; if it exists in one, it exists in the other one as well)
	return_value = edge_exists(g->outIndex, from, to);
	if (return_value < 0)
	{	// error
		return return_value;
	}
	else if (!return_value)
	{	// edge does not exist
		return_value = add_edge(g->outIndex, from, to);
		if (return_value) return return_value;
		return_value = add_edge(g->inIndex, to, from);
		if (return_value) return return_value;
	}
	// if return_value == 1 the edge already exists, so there is nothing to be done
	error_val = OK_SUCCESS;
	return OK_SUCCESS;
}

rcode gFindShortestPath(pGraph g, graphNode from, graphNode to/*, &solution*/)
{
	if (g == NULL)
	{
		error_val = GRAPH_NULL_POINTER_PROVIDED;
		return GRAPH_NULL_POINTER_PROVIDED;
	}

	error_val = OK_SUCCESS;
	return OK_SUCCESS;
}

rcode gDestroyGraph(pGraph *g)
{
	if (g == NULL)
	{
		error_val = GRAPH_NULL_POINTER_PROVIDED;
		return GRAPH_NULL_POINTER_PROVIDED;
	}
	destroyNodeIndex((*g)->outIndex);
	destroyNodeIndex((*g)->inIndex);
	free(*g);
	*g = NULL;
	error_val = OK_SUCCESS;
	return OK_SUCCESS;
}
