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
	rcode return_value;
	if (g == NULL)
	{
		error_val = GRAPH_NULL_POINTER_PROVIDED;
		return GRAPH_NULL_POINTER_PROVIDED;
	}
	return_value = add_edge(g->outIndex, from, to);
	if (return_value && return_value != INDEX_NEIGHBOR_EXISTS)
		return return_value;
	return_value = add_edge(g->inIndex, to, from);
	if (return_value && return_value != INDEX_NEIGHBOR_EXISTS)
		return return_value;
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
