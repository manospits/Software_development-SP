#include <stdlib.h>
#include "graph.h"
#include "buffer.h"
#include "index.h"
#include "hash.h"
#include "intlist.h"

typedef struct graph
{
	Index_ptr inIndex;
	pBuffer inBuffer;
	Index_ptr outIndex;
	pBuffer outBuffer;
}graph;

rcode gCreateGraph(pGraph *g)
{
	if (g == NULL || *g == NULL)
	{
		error_val = GRAPH_NULL_POINTER_PROVIDED;
		return GRAPH_NULL_POINTER_PROVIDED;
	}
	if ((*g = malloc(sizeof(graph))) == NULL)
	{
		error_val = GRAPH_CREATION_BASIC_STRUCT_MALLOC_FAIL;
		return GRAPH_CREATION_BASIC_STRUCT_MALLOC_FAIL;
	}
	if (((*g)->inIndex = createNodeIndex()) == NULL)
	{
		free(*g);
		*g = NULL;
		error_val = GRAPH_CREATION_INDEX_MALLOC_FAIL;
		return GRAPH_CREATION_INDEX_MALLOC_FAIL;
	}
	if (((*g)->outIndex = createNodeIndex()) == NULL);
	{
		destroyNodeIndex((*g)->inIndex);
		free(*g);
		*g = NULL;
		error_val = GRAPH_CREATION_INDEX_MALLOC_FAIL;
		return GRAPH_CREATION_INDEX_MALLOC_FAIL;
	}
	if (((*g)->inBuffer = createBuffer()) == NULL)
	{
		destroyNodeIndex((*g)->outIndex);
		destroyNodeIndex((*g)->inIndex);
		free(*g);
		*g = NULL;
		error_val = GRAPH_CREATION_BUFFER_MALLOC_FAIL;
		return GRAPH_CREATION_BUFFER_MALLOC_FAIL;
	}
	if (((*g)->outBuffer = createBuffer()) == NULL)
	{
		destroyBuffer((*g)->inBuffer);
		destroyNodeIndex((*g)->outIndex);
		destroyNodeIndex((*g)->inIndex);
		free(*g);
		*g = NULL;
		error_val = GRAPH_CREATION_BUFFER_MALLOC_FAIL;
		return GRAPH_CREATION_BUFFER_MALLOC_FAIL;
	}
	error_val = OK_SUCCESS;
	return OK_SUCCESS;
}

rcode gAddNode(pGraph g, graphNode from, graphNode to)
{
	if (g == NULL)
	{
		error_val = GRAPH_NULL_POINTER_PROVIDED;
		return GRAPH_NULL_POINTER_PROVIDED;
	}

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
	destroyBuffer((*g)->inBuffer);
	destroyBuffer((*g)->outBuffer);
	destroyNodeIndex((*g)->outIndex);
	destroyNodeIndex((*g)->inIndex);
	free(*g);
	*g = NULL;
	error_val = OK_SUCCESS;
	return OK_SUCCESS;
}
