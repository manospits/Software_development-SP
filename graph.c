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
	if (g == NULL) return GRAPH_NULL_POINTER_PROVIDED;
	if ((*g = malloc(sizeof(graph))) == NULL)
	{
		return GRAPH_CREATION_BASIC_STRUCT_MALLOC_FAIL;
	}
	if (((*g)->inIndex = createNodeIndex()) == NULL)
	{
		free(*g);
		return GRAPH_CREATION_INDEX_MALLOC_FAIL;
	}
	if (((*g)->outIndex = createNodeIndex()) == NULL);
	{
		free((*g)->inIndex);
		free(*g);
		return GRAPH_CREATION_INDEX_MALLOC_FAIL;
	}
	if (((*g)->inBuffer = createBuffer()) == NULL)
	{
		free((*g)->outIndex);
		free((*g)->inIndex);
		free(*g);
		return GRAPH_CREATION_BUFFER_MALLOC_FAIL;
	}
	if (((*g)->outBuffer = createBuffer()) == NULL)
	{
		free((*g)->inBuffer);
		free((*g)->outIndex);
		free((*g)->inIndex);
		free(*g);
		return GRAPH_CREATION_BUFFER_MALLOC_FAIL;
	}
	return OK_SUCCESS;
}

rcode gAddNode(pGraph g, graphNode from, graphNode to)
{
	return OK_SUCCESS;
}

rcode gFindShortestPath(pGraph g, graphNode from, graphNode to/*, &solution*/)
{
	return OK_SUCCESS;
}

rcode gDestroyGraph(pGraph g)
{
	return OK_SUCCESS;
}
