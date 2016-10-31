#include <stdlib.h>
#include "graph.h"
#include "buffer.h"
#include "index.h"
#include "hash.h"
#include "intlist.h"

#define HASHTABLE_SIZE 50

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

int bfs(pGraph g, graphNode from, graphNode to);
int bidirectional_bfs(pGraph g, graphNode from, graphNode to);

int gFindShortestPath(pGraph g, graphNode from, graphNode to)
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
	return bfs(g, from, to);
	error_val = OK_SUCCESS;
	return OK_SUCCESS;
}

int calculate_hashtable_size(pGraph g)
{	// returns the maximum of {10, number_of_g's_nodes/2}

}

int hash_function(void *data)
{
	return (*(uint32_t *)data) % HASHTABLE_SIZE;
}

int bfs(pGraph g, graphNode from, graphNode to)
{
	phash visited;
	phead open_list;
	if ((visited = create_hashtable(HASHTABLE_SIZE, &hash_function, 1)) == NULL)
	{
		// error_val not set here, so print_error() will prin the error from create_hashtable()
		return GRAPH_SEARCH_INIT_STRUCTS_FAIL;
	}
	if ((open_list = cr_list()) == NULL)
	{
		// error_val not set here, so print_error() will prin the error from create_hashtable()
		return GRAPH_SEARCH_INIT_STRUCTS_FAIL;
	}
}

int bidirectional_bfs(pGraph g, graphNode from, graphNode to)
{

}
