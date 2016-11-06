#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "buffer.h"
#include "index.h"
#include "hash.h"
#include "struct_list.h"
#include "intlist.h"
#include <assert.h>

#define HASHTABLE_SIZE 100003

int hash_function(void *data,void *size);

    typedef struct graph
{
	Index_ptr inIndex;
	Index_ptr outIndex;
    int hash_additions;     //total hash_additions
    int queries;            //total queries
    phash visited;
}_graph;

pGraph gCreateGraph()
{
	pGraph g;
	if ((g = malloc(sizeof(_graph))) == NULL)
	{
		error_val = GRAPH_CREATION_BASIC_STRUCT_MALLOC_FAIL;
		return NULL;
	}
    g->visited=NULL;
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
    g->hash_additions=0;
    g->queries=0;
	return g;
}

rcode gDestroyGraph(pGraph *g)
{
	if (g == NULL)
	{
		error_val = GRAPH_NULL_POINTER_PROVIDED;
		return GRAPH_NULL_POINTER_PROVIDED;
	}
    if( (*g)->visited!=NULL && ds_hash((*g)->visited)<0 ){
        print_error();
        error_val =GRAPH_HASH_DESTROY_FAIL;
        return GRAPH_HASH_DESTROY_FAIL;
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
	// check if edge exists
	// we check in the index that has the fewer neighbors, so that extreme cases that slow the process are avoided
	// (no need to check both indexes; if it exists in one, it exists in the other one as well)
	if(get_node_number_of_edges(g->outIndex,from) <= get_node_number_of_edges(g->inIndex,to))
        return_value = edge_exists(g->outIndex, from, to);
    else
        return_value = edge_exists(g->inIndex, to, from);

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
    int return_value;
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
    if(g->visited==NULL)
        g->visited=create_hashtable(HASHTABLE_SIZE,hash_function,1);
    (type == BFS ? (return_value=bfs(g, from, to)) : (return_value=bidirectional_bfs(g, from, to)));
    g->queries++;
    ds_hash(g->visited);
    g->visited=NULL;
    return return_value;
}

int hash_function(void *data,void *size)
{	// TODO change HASHTABLE_SIZE with calculate_hashtable_size() to better adjust to greater amount of data
	return (*(uint32_t *)data) % (*(int*)size);
}

int bfs(pGraph g, graphNode from, graphNode to)
{
	stphead open_list;
	int i, return_value, temp_node_tag;
	graphNode temp_node;
	pBuffer temp_buffer;
	ptr buffer_ptr_to_listnode;
	plnode listnode;
	// TODO change 1 to 0 with greater hash size to see if it adjust better in greater amount of data
	if ((open_list = st_cr_list()) == NULL)
	{
	    // save error_value
	    return_value = error_val;
		// and restore it
		error_val = return_value;
		// error_val not set here, so print_error() will print the error from create_hashtable()
		return GRAPH_SEARCH_INIT_STRUCTS_FAIL;
	}
	if ((return_value = st_insert_back(open_list, from, 0)) != OK_SUCCESS)
	{
		st_ds_list(open_list);
        // update error_value changed by destroy_<>() functions
		error_val = return_value;
		return return_value;
	}
	while(st_get_size(open_list) > 0)
	{
		if ((temp_node = st_peek(open_list)) < 0)
		{
			st_ds_list(open_list);
			error_val = temp_node;
			return temp_node;
		}
		if ((temp_node_tag = st_get_tag(open_list, temp_node)) < 0)
        {
			st_ds_list(open_list);
			error_val = temp_node_tag;
			return temp_node_tag;
        }
		if (temp_node == to)
		{	// target node found
			st_ds_list(open_list);
			error_val = OK_SUCCESS;
			return temp_node_tag;
		}
		if ((return_value = st_pop_front(open_list)) < 0)
		{
			st_ds_list(open_list);
			error_val = return_value;
			return return_value;
		}
		// check if current node is already visited
		return_value = in_hash(g->visited, temp_node);
		if (return_value < 0)
		{
			st_ds_list(open_list);
			error_val = return_value;
			return return_value;
		}
		if (return_value > 0)
			continue;	// node already visited
        // if return_value == 0, then the node hasn't been visited yet, so we visit & expand him
		if ((return_value = h_insert(g->visited, temp_node, 0)) < 0)
		{
			st_ds_list(open_list);
			error_val = return_value;
			return return_value;
		}
        // add to number of total additions the above addition
        g->hash_additions++;
		buffer_ptr_to_listnode = getListHead(g->outIndex, temp_node);
		if (buffer_ptr_to_listnode == -1)
        {   // node has no neighbors
            continue;
        }
		else if (buffer_ptr_to_listnode < 0)
		{   // an error occurred
			st_ds_list(open_list);
			error_val = return_value;
			return buffer_ptr_to_listnode;
		}
		if ((temp_buffer = return_buffer(g->outIndex)) == NULL)
		{
			return_value = error_val;
			st_ds_list(open_list);
			error_val = return_value;
			return return_value;
		}
		if ((listnode = getListNode(temp_buffer, buffer_ptr_to_listnode)) == NULL)
		{
			return_value = error_val;
			st_ds_list(open_list);
			error_val = return_value;
			return return_value;
		}
		i = 0;
		while (listnode->neighbor[i] != -1)
		{
			return_value = in_hash(g->visited, listnode->neighbor[i]);
			if (return_value < 0)
			{
				st_ds_list(open_list);
				error_val = return_value;
				return return_value;
			}
			if (!return_value)
			{	// if this node hasn't been visited yet
				if ((return_value = st_insert_back(open_list, listnode->neighbor[i], temp_node_tag+1)) != OK_SUCCESS)
				{
					st_ds_list(open_list);
					error_val = return_value;
					return return_value;
				}
			}
			i++;
			if (i == N)
			{
			    if (listnode->nextListNode == -1)   // if there are no more neighbors, break
                    break;
				if ((listnode = getListNode(temp_buffer, listnode->nextListNode)) == NULL)
				{
					return_value = error_val;
					st_ds_list(open_list);
					error_val = return_value;
					return return_value;
				}
				i = 0;
			}
		}
	}
	// check if control exited the loop because of an error instead of just empty list
	if ((return_value = st_get_size(open_list)) < 0)
	{
		st_ds_list(open_list);
		error_val = return_value;
		return return_value;
	}
	st_ds_list(open_list);
	error_val = OK_SUCCESS;
	return GRAPH_SEARCH_PATH_NOT_FOUND;
}

int bidirectional_bfs(pGraph g, graphNode from, graphNode to)
{	// 0 is for out-Index, 1 is for in-Index
	phead open_list[2];
	int i, n, return_value, path_length[2], number_of_nodes, current;
	char path_found = 0;
	graphNode temp_node;
	pBuffer temp_buffer;
	ptr buffer_ptr_to_listnode;
	plnode listnode;
	if ((open_list[0] = cr_list()) == NULL)
	{
	    // save error_val
	    return_value = error_val;
		// and restore it
		error_val = return_value;
		// error_val not set here, so print_error() will print the error from create_hashtable()
		return GRAPH_SEARCH_INIT_STRUCTS_FAIL;
	}
	if ((open_list[1] = cr_list()) == NULL)
	{
	    return_value = error_val;
		ds_list(open_list[0]);
		error_val = return_value;
		// error_val not set here, so print_error() will print the error from create_hashtable()
		return GRAPH_SEARCH_INIT_STRUCTS_FAIL;
	}
	if ((return_value = insert_back(open_list[0], from)) != OK_SUCCESS)
	{
		ds_list(open_list[0]);
		ds_list(open_list[1]);
		error_val = return_value;
		return return_value;
	}
	if ((return_value = insert_back(open_list[1], to)) != OK_SUCCESS)
	{
		ds_list(open_list[0]);
		ds_list(open_list[1]);
		error_val = return_value;
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
			ds_list(open_list[0]);
			ds_list(open_list[1]);
			error_val = number_of_nodes;
			return number_of_nodes;
		}
		for (n = 0 ; n < number_of_nodes ; ++n)
		{
			if ((temp_node = peek(open_list[current])) < 0)
			{
				ds_list(open_list[0]);
				ds_list(open_list[1]);
				error_val = temp_node;
				return temp_node;
			}
			if ((return_value = pop_front(open_list[current])) < 0)
			{
				ds_list(open_list[0]);
				ds_list(open_list[1]);
				error_val = return_value;
				return return_value;
			}
			return_value = in_hash(g->visited, temp_node);
			if (return_value < 0)
			{
				ds_list(open_list[0]);
				ds_list(open_list[1]);
				error_val = return_value;
				return return_value;
			}
			if (return_value > 0)
			{
				if ((return_value = ret_tag(g->visited, temp_node)) < 0)
				{
                    ds_list(open_list[0]);
                    ds_list(open_list[1]);
                    error_val = return_value;
                    return return_value;
				}
				//assert(return_value != 1-current);    // DEBUG
				// if the node was found in the 'visited' hashtable, then its tag will be 'current'
				// It can't be 1-current, because if it was, it would have been recognized before it entered the 'visited', and the search would have stopped (a path would have been found)
				// Therefore, the tag is equal to current, which means that the node has already been visited by this bfs, so we don't expand it; instead, we continue
				continue;
			}
			// if return_value == 0 then the node hasn't been visited yet, so proceed as normal
			if ((return_value = h_insert(g->visited, temp_node, current)) < 0)
			{
				ds_list(open_list[0]);
				ds_list(open_list[1]);
				error_val = return_value;
				return return_value;
			}
            g->hash_additions++;
			buffer_ptr_to_listnode = getListHead((current == 0 ? g->outIndex : g->inIndex), temp_node);
            if (buffer_ptr_to_listnode == -1)
            {   // node has no neighbors
                continue;
            }
            else if (buffer_ptr_to_listnode < 0)
            {   // an error occurred
				ds_list(open_list[0]);
				ds_list(open_list[1]);
				error_val = buffer_ptr_to_listnode;
				return buffer_ptr_to_listnode;
			}
			if ((temp_buffer = return_buffer((current == 0 ? g->outIndex : g->inIndex))) == NULL)
			{
				return_value = error_val;
				ds_list(open_list[0]);
				ds_list(open_list[1]);
				error_val = return_value;
				return return_value;
			}
			if ((listnode = getListNode(temp_buffer, buffer_ptr_to_listnode)) == NULL)
			{
				return_value = error_val;
				ds_list(open_list[0]);
				ds_list(open_list[1]);
				error_val = return_value;
				return return_value;
			}
			i = 0;
			while (listnode->neighbor[i] != -1)
			{
				// check if the two nodes (from-to) are directly connected
				if (path_length[current] == 1 && current == 0 && listnode->neighbor[i] == to)
				{	// the two nodes are direct neighbors, so the path is 1
					ds_list(open_list[0]);
					ds_list(open_list[1]);
					return 1;
				}
				return_value = in_hash(g->visited, listnode->neighbor[i]);
				if (return_value < 0)
				{
					ds_list(open_list[0]);
					ds_list(open_list[1]);
					error_val = return_value;
					return return_value;
				}
				else if (!return_value)
				{	// if this node hasn't been visited yet, insert it to the list
					if ((return_value = insert_back(open_list[current], listnode->neighbor[i])) != OK_SUCCESS)
					{
						ds_list(open_list[0]);
						ds_list(open_list[1]);
						error_val = return_value;
						return return_value;
					}
				}
				else if (return_value > 0)
				{
					return_value = ret_tag(g->visited, listnode->neighbor[i]);
					if (return_value < 0)
					{
						ds_list(open_list[0]);
						ds_list(open_list[1]);
						error_val = return_value;
						return return_value;
					}
					if (return_value == 1-current)
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
					if (listnode->nextListNode == -1)   // if there are no more neighbors, break
                        break;
                    if ((listnode = getListNode(temp_buffer, listnode->nextListNode)) == NULL)
					{
						return_value = error_val;
						ds_list(open_list[0]);
						ds_list(open_list[1]);
						error_val = return_value;
						return return_value;
					}
					i = 0;
				}
			}
			if (path_found)
				break;
		}
		if (path_found)
            break;
	}
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
