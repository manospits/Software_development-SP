#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "buffer.h"
#include "index.h"
#include "hash.h"
#include "intlist.h"
#include <assert.h>

#define HASHTABLE_SIZE 50
#define STARTING_HASHTABLE_SIZE 20	//used by calculate_hashtable_size()

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

rcode gAddEdge(pGraph g, graphNode from, graphNode to)
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

int gFindShortestPath(pGraph g, graphNode from, graphNode to, int type)
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
	return (type == BFS ? bfs(g, from, to) : bidirectional_bfs(g, from, to));
}

int calculate_hashtable_size(pGraph g)
{	// returns the maximum of {10, number_of_g's_nodes/2}
	return 0;
}

int hash_function(void *data)
{	// TODO change HASHTABLE_SIZE with calculate_hashtable_size() to better adjust to greater amount of data
	return (*(uint32_t *)data) % HASHTABLE_SIZE;
}

int bfs(pGraph g, graphNode from, graphNode to)
{
	phash visited;
	phead open_list;
	int i, return_value, path_length;
	graphNode temp_node;
	pBuffer temp_buffer;
	ptr buffer_ptr_to_listnode;
	plnode listnode;
	// TODO change 1 to 0 with greater hash size to see if it adjust better in greater amount of data
	if ((visited = create_hashtable(HASHTABLE_SIZE, &hash_function, 1)) == NULL)
	{
		// error_val not set here, so print_error() will print the error from create_hashtable()
		return GRAPH_SEARCH_INIT_STRUCTS_FAIL;
	}
	if ((open_list = cr_list()) == NULL)
	{
		ds_hash(visited);
		// error_val not set here, so print_error() will print the error from create_hashtable()
		return GRAPH_SEARCH_INIT_STRUCTS_FAIL;
	}
	if ((return_value = insert_back(open_list, from)) != OK_SUCCESS)
	{
		ds_hash(visited);
		ds_list(open_list);
		return return_value;
	}
	path_length = 0;
	while(get_size(open_list) > 0)
	{
		path_length++;
		if ((temp_node = peek(open_list)) < 0)
		{
			ds_hash(visited);
			ds_list(open_list);
			return temp_node;
		}
		if ((return_value = pop_front(open_list)) < 0)
		{
			ds_hash(visited);
			ds_list(open_list);
			return return_value;
		}
		if (temp_node == to)
		{	// target node found
			ds_hash(visited);
			ds_list(open_list);
			error_val = OK_SUCCESS;
			return path_length-1;
		}
		// check if current node is already visited
		return_value = in_hash(visited, temp_node);
		if (return_value < 0)
		{
			ds_hash(visited);
			ds_list(open_list);
			return return_value;
		}
		if (return_value > 0)
			continue;	// node already visited
		if ((return_value = h_insert(visited, temp_node, 0)) < 0)
		{
			ds_hash(visited);
			ds_list(open_list);
			return return_value;
		}
		if ((buffer_ptr_to_listnode = getListHead(g->outIndex, temp_node)) < 0)
		{
			ds_hash(visited);
			ds_list(open_list);
			return buffer_ptr_to_listnode;
		}
		if ((temp_buffer = return_buffer(g->outIndex)) == NULL)
		{
			return_value = error_val;
			ds_hash(visited);
			ds_list(open_list);
			return return_value;
		}
		if ((listnode = getListNode(temp_buffer, buffer_ptr_to_listnode)) == NULL)
		{
			return_value = error_val;
			ds_hash(visited);
			ds_list(open_list);
			return return_value;
		}
		i = 0;
		while (listnode->neighbor[i] != -1)
		{
			return_value = in_hash(visited, listnode->neighbor[i]);
			if (return_value < 0)
			{
				ds_hash(visited);
				ds_list(open_list);
				return return_value;
			}
			if (!return_value)
			{	// if this node hasn't been visited yet
				if ((return_value = insert_back(open_list, listnode->neighbor[i])) != OK_SUCCESS)
				{
					ds_hash(visited);
					ds_list(open_list);
					return return_value;
				}
			}
			i++;
			if (i == N)
			{
				if ((listnode = getListNode(temp_buffer, listnode->nextListNode)) == NULL)
				{
					return_value = error_val;
					ds_hash(visited);
					ds_list(open_list);
					return return_value;
				}
				i = 0;
			}
		}
	}
	// check if control exited the loop because of an error instead of just empty list
	if ((return_value = get_size(open_list)) < 0)
	{
		ds_hash(visited);
		ds_list(open_list);
		return return_value;
	}
	ds_hash(visited);
	ds_list(open_list);
	error_val = OK_SUCCESS;
	return GRAPH_SEARCH_PATH_NOT_FOUND;
}

int bidirectional_bfs(pGraph g, graphNode from, graphNode to)
{	// 0 is for out-Index, 1 is for in-Index
	phash visited;
	phead open_list[2];
	int i, n, return_value, path_length[2], number_of_nodes, current;
	char path_found = 0;
	graphNode temp_node;
	pBuffer temp_buffer;
	ptr buffer_ptr_to_listnode;
	plnode listnode;
	if ((visited = create_hashtable(HASHTABLE_SIZE, &hash_function, 1)) == NULL)
	{
		// error_val not set here, so print_error() will print the error from create_hashtable()
		return GRAPH_SEARCH_INIT_STRUCTS_FAIL;
	}
	if ((open_list[0] = cr_list()) == NULL)
	{
		ds_hash(visited);
		// error_val not set here, so print_error() will print the error from create_hashtable()
		return GRAPH_SEARCH_INIT_STRUCTS_FAIL;
	}
	if ((open_list[1] = cr_list()) == NULL)
	{
		ds_list(open_list[0]);
		ds_hash(visited);
		// error_val not set here, so print_error() will print the error from create_hashtable()
		return GRAPH_SEARCH_INIT_STRUCTS_FAIL;
	}
	if ((return_value = insert_back(open_list[0], from)) != OK_SUCCESS)
	{
		ds_hash(visited);
		ds_list(open_list[0]);
		ds_list(open_list[1]);
		return return_value;
	}
	if ((return_value = insert_back(open_list[1], to)) != OK_SUCCESS)
	{
		ds_hash(visited);
		ds_list(open_list[0]);
		ds_list(open_list[1]);
		return return_value;
	}
	path_length[0] = 0;
	path_length[1] = 0;
	current = 1;
	while(get_size(open_list[0]) > 0 && get_size(open_list[1]) > 0)
	{
		current = 1-current;
		path_length[current]++;
		if ((number_of_nodes = get_size(open_list[current])) < 0)
		{
			ds_hash(visited);
			ds_list(open_list[0]);
			ds_list(open_list[1]);
			return number_of_nodes;
		}
		for (n = 0 ; n < number_of_nodes ; ++n)
		{
			if ((temp_node = peek(open_list[current])) < 0)
			{
				ds_hash(visited);
				ds_list(open_list[0]);
				ds_list(open_list[1]);
				return temp_node;
			}
			if ((return_value = pop_front(open_list[current])) < 0)
			{
				ds_hash(visited);
				ds_list(open_list[0]);
				ds_list(open_list[1]);
				return return_value;
			}
			return_value = in_hash(visited, temp_node);
			if (return_value < 0)
			{
				ds_hash(visited);
				ds_list(open_list[0]);
				ds_list(open_list[1]);
				return return_value;
			}
			if (return_value > 0)
			{
				if ((return_value = ret_tag(visited, temp_node)) < 0)
				{

				}
				assert(return_value == current-1);
				// return value (=tag) == current - the node has been visited before by this bfs
				continue;
			}
			// if return_value == 0 then the node hasn't been visited yet, so proceed as normal
			/*if (temp_node == to)
			{	// target node found
				ds_hash(visited);
				ds_list(open_list[0]);
				ds_list(open_list[1]);
				error_val = OK_SUCCESS;
				return path_length;
			}*/
			if ((return_value = h_insert(visited, temp_node, current)) < 0)
			{
				ds_hash(visited);
				ds_list(open_list[0]);
				ds_list(open_list[1]);
				return return_value;
			}
			if ((buffer_ptr_to_listnode = getListHead((current == 0 ? g->outIndex : g->inIndex), temp_node)) < 0)
			{
				ds_hash(visited);
				ds_list(open_list[0]);
				ds_list(open_list[1]);
				return buffer_ptr_to_listnode;
			}
			if ((temp_buffer = return_buffer((current == 0 ? g->outIndex : g->inIndex))) == NULL)
			{
				return_value = error_val;
				ds_hash(visited);
				ds_list(open_list[0]);
				ds_list(open_list[1]);
				return return_value;
			}
			if ((listnode = getListNode(temp_buffer, buffer_ptr_to_listnode)) == NULL)
			{
				return_value = error_val;
				ds_hash(visited);
				ds_list(open_list[0]);
				ds_list(open_list[1]);
				return return_value;
			}
			i = 0;
			while (listnode->neighbor[i] != -1)
			{
				// check if the two nodes (from-to) are directly connected
				if (path_length[current] == 1 && current == 0 && listnode->neighbor[i] == to)
				{	// the two nodes are direct neighbors, so the path is 1
					ds_hash(visited);
					ds_list(open_list[0]);
					ds_list(open_list[1]);
					return 1;
				}
				return_value = in_hash(visited, listnode->neighbor[i]);
				if (return_value < 0)
				{
					ds_hash(visited);
					ds_list(open_list[0]);
					ds_list(open_list[1]);
					return return_value;
				}
				else if (!return_value)
				{	// if this node hasn't been visited yet, insert it to the list
					if ((return_value = insert_back(open_list[current], listnode->neighbor[i])) != OK_SUCCESS)
					{
						ds_hash(visited);
						ds_list(open_list[0]);
						ds_list(open_list[1]);
						return return_value;
					}
				}
				else if (return_value > 0)
				{
					return_value = ret_tag(visited, listnode->neighbor[i]);
					if (return_value < 0)
					{
						ds_hash(visited);
						ds_list(open_list[0]);
						ds_list(open_list[1]);
						return return_value;
					}
					if (return_value == current-1)
					{	// if the tag of the neighbor that's already on the 'visited' list is the other bfs' tag
						// then the two bfss have just met so a path has been found
						path_found = 1;
						break;
					}
					// if return value (=tag) == current, then the node has already been visited by this bfs, so it doesn't enter the open list again
				}
				i++;
				if (i == N)
				{
					if ((listnode = getListNode(temp_buffer, listnode->nextListNode)) == NULL)
					{
						return_value = error_val;
						ds_hash(visited);
						ds_list(open_list[0]);
						ds_list(open_list[1]);
						return return_value;
					}
					i = 0;
				}
			}
			if (path_found)
				break;
		}
	}
	ds_hash(visited);
	ds_list(open_list[0]);
	ds_list(open_list[1]);
	if (path_found)
		return path_length[0] + path_length[1] - 1;
	else
		return GRAPH_SEARCH_PATH_NOT_FOUND;
}

void gPrintGraph(pGraph g)
{
	pBuffer temp_buffer;
	ptr buffer_ptr_to_listnode;
	plnode listnode;
	int i, j, size = get_index_size(g->outIndex);
	if (size < 0)
	{
		print_error();
		return;
	}
	printf("Printing graph...\n");
	for (i = 0 ; i < size ; ++i)
	{
		printf("Node %d\n\tOutcoming:", i);
		if ((buffer_ptr_to_listnode = getListHead(g->outIndex, i)) < 0 && buffer_ptr_to_listnode != -1)
		{
			puts("\n");
			print_error();
			return;
		}
		if (buffer_ptr_to_listnode != -1)
		{
			if ((temp_buffer = return_buffer(g->outIndex)) == NULL)
			{
				puts("\n");
				print_error();
				return;
			}
			if ((listnode = getListNode(temp_buffer, buffer_ptr_to_listnode)) == NULL)
			{
				puts("\n");
				print_error();
				return;
			}
			j = 0;
			while (listnode->neighbor[j] != -1)
			{
				printf(" %d", listnode->neighbor[j]);
				j++;
				if (j == N)
				{
					if ((listnode = getListNode(temp_buffer, listnode->nextListNode)) == NULL)
					{
						puts("\n");
						print_error();
						return;
					}
					j = 0;
				}
			}
		}
		printf("\n\tIncoming:");
		if ((buffer_ptr_to_listnode = getListHead(g->inIndex, i)) < 0 && buffer_ptr_to_listnode != -1)
		{
			puts("\n");
			print_error();
			return;
		}
		if (buffer_ptr_to_listnode != -1)
		{
			if ((temp_buffer = return_buffer(g->inIndex)) == NULL)
			{
				puts("\n");
				print_error();
				return;
			}
			if ((listnode = getListNode(temp_buffer, buffer_ptr_to_listnode)) == NULL)
			{
				puts("\n");
				print_error();
				return;
			}
			j = 0;
			while (listnode->neighbor[j] != -1)
			{
				printf(" %d", listnode->neighbor[j]);
				j++;
				if (j == N)
				{
					if ((listnode = getListNode(temp_buffer, listnode->nextListNode)) == NULL)
					{
						puts("\n");
						print_error();
						return;
					}
					j = 0;
				}
			}
		}
		puts("");
	}
}
