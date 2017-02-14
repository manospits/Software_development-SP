#include <stdio.h>
#include <stdlib.h>
#include "../include/graph.h"
#include "../include/buffer.h"
#include "../include/index.h"
#include "../include/visited.h"
#include "../include/struct_list.h"
#include "../include/intlist.h"
#include "../include/scc.h"
#include "../include/CCindex.h"
#include "../include/scc.h"
#include "../include/grail.h"
#include <assert.h>

#define VISITED 0
#define EXPANDED 1

typedef struct graph
{
    Index_ptr inIndex;
    Index_ptr outIndex;
    pvis visited;
    phead open_intlist[2];
    CC_index ccindex;
    pSCC sccs;
    Grail grail;
    stphead open_list;
    int type;
}_graph;

pGraph gCreateGraph()
{
    pGraph g;
    if ((g = malloc(sizeof(_graph))) == NULL)
    {
        //error_val = GRAPH_CREATION_BASIC_STRUCT_MALLOC_FAIL;
        print_errorv(GRAPH_CREATION_BASIC_STRUCT_MALLOC_FAIL);
        return NULL;
    }
    g->visited=NULL;
    g->open_list=NULL;
    g->open_intlist[0]=NULL;
    g->open_intlist[1]=NULL;
    g->ccindex=NULL;
    g->type=NOTYPE;
    if ((g->inIndex = createNodeIndex()) == NULL)
    {
        free(g);
        //error_val = GRAPH_CREATION_INDEX_MALLOC_FAIL;
        print_errorv(GRAPH_CREATION_INDEX_MALLOC_FAIL);
        return NULL;
    }
    if ((g->outIndex = createNodeIndex()) == NULL)
    {
        destroyNodeIndex(g->inIndex);
        free(g);
        //error_val = GRAPH_CREATION_INDEX_MALLOC_FAIL;
        print_errorv(GRAPH_CREATION_INDEX_MALLOC_FAIL);
        return NULL;
    }
    if ((g->open_intlist[0] = cr_list()) == NULL)
    {
        //error_val = GRAPH_CREATION_CR_LIST_FAIL;
        print_errorv(GRAPH_CREATION_CR_LIST_FAIL);
        return NULL;
    }
    if ((g->open_intlist[1] = cr_list()) == NULL)
    {
        //error_val = GRAPH_CREATION_CR_LIST_FAIL;
        print_errorv(GRAPH_CREATION_CR_LIST_FAIL);
        return NULL;
    }
    return g;
}

rcode gDestroyGraph(pGraph *g)
{
    if (g == NULL)
    {
        //error_val = GRAPH_NULL_POINTER_PROVIDED;
        print_errorv(GRAPH_NULL_POINTER_PROVIDED);
        return GRAPH_NULL_POINTER_PROVIDED;
    }
    if( (*g)->visited!=NULL && v_ds_visited((*g)->visited) <0 ){
        //print_error();
        //error_val = GRAPH_HASH_DESTROY_FAIL;
        print_errorv(GRAPH_HASH_DESTROY_FAIL);
        return GRAPH_HASH_DESTROY_FAIL;
    }
    if(((*g)->open_list!=NULL)){
        st_ds_list((*g)->open_list);
    }
    if(((*g)->open_intlist[0]!=NULL)){
        ds_list((*g)->open_intlist[0]);
    }
    if(((*g)->open_intlist[1]!=NULL)){
        ds_list((*g)->open_intlist[1]);
    }
    if((*g)->type==DYNAMIC){
        CC_destroy((*g)->ccindex);
    }
    else if((*g)->type==STATIC){
        destroyStronglyConnectedComponents((*g)->sccs);
        destroyGrailIndex((*g)->grail);
    }
    destroyNodeIndex((*g)->outIndex);
    destroyNodeIndex((*g)->inIndex);
    free(*g);
    *g = NULL;
    //error_val = OK_SUCCESS;
    return OK_SUCCESS;
}

rcode gAddEdge(pGraph g, graphNode from, graphNode to)
{
    if (g == NULL)
    {
        //error_val = GRAPH_NULL_POINTER_PROVIDED;
        print_errorv(GRAPH_NULL_POINTER_PROVIDED);
        return GRAPH_NULL_POINTER_PROVIDED;
    }
    if (from == to)
    {
        //error_val = OK_SUCCESS;
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
    {    // error
        return return_value;
    }
    else if (!return_value)
    {    // edge does not exist
        return_value = add_edge(g->outIndex, from, to);
        if (return_value) return return_value;
        return_value = add_edge(g->inIndex, to, from);
        if (return_value) return return_value;
        if(g->type==DYNAMIC){
            CC_insertNewEdge(g->ccindex,from,to);
        }
    }
    else{
        return EDGE_EXISTS;
    }
    // if return_value == 1 the edge already exists, so there is nothing to be done
    //error_val = OK_SUCCESS;
    return OK_SUCCESS;
}

rcode gAddEdge_t(pGraph g, graphNode from, graphNode to,uint32_t version)
{
    if (g == NULL)
    {
        //error_val = GRAPH_NULL_POINTER_PROVIDED;
        print_errorv(GRAPH_NULL_POINTER_PROVIDED);
        return GRAPH_NULL_POINTER_PROVIDED;
    }
    if (from == to)
    {
        //error_val = OK_SUCCESS;
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
    {    // error
        return return_value;
    }
    else if (!return_value)
    {    // edge does not exist
        return_value = add_edge_t(g->outIndex, from, to,version);
        if (return_value) return return_value;
        return_value = add_edge_t(g->inIndex, to, from,version);
        if (return_value) return return_value;
        if(g->type==DYNAMIC){
            CC_insertNewEdge_t(g->ccindex,from,to,version);
        }
    }
    else{
        return EDGE_EXISTS;
    }
    // if return_value == 1 the edge already exists, so there is nothing to be done
    //error_val = OK_SUCCESS;
    return OK_SUCCESS;
}
int bfs(pGraph g, graphNode from, graphNode to);
int bidirectional_bfs(pGraph g, graphNode from, graphNode to, phead *open_list, pvis visited,uint32_t version);
int bidirectional_bfs_grail(pGraph g, graphNode from, graphNode to, phead *open_list, pvis visited);

int gFindShortestPath(pGraph g, graphNode from, graphNode to, int type)
{
    int return_value;
    if (g == NULL)
    {
        //error_val = GRAPH_NULL_POINTER_PROVIDED;
        print_errorv(GRAPH_NULL_POINTER_PROVIDED);
        return GRAPH_NULL_POINTER_PROVIDED;
    }
    if (from == to)
    {
        //error_val = OK_SUCCESS;
        return OK_SUCCESS;
    }
    if(g->visited==NULL)
        g->visited=create_visited(get_index_size(g->inIndex));
    if(type == BFS && g->open_list==NULL){
        if ((g->open_list = st_cr_list()) == NULL)
        {
            //error_val = GRAPH_SEARCH_INIT_STRUCTS_FAIL;
            print_errorv(GRAPH_SEARCH_INIT_STRUCTS_FAIL);
            return GRAPH_SEARCH_INIT_STRUCTS_FAIL;
        }
    }
    else if(g->open_intlist[0]==NULL && g->open_intlist[1]==NULL){
        if ((g->open_intlist[0] = cr_list()) == NULL)
        {
            //error_val = GRAPH_SEARCH_INIT_STRUCTS_FAIL;
            print_errorv(GRAPH_SEARCH_INIT_STRUCTS_FAIL);
            return GRAPH_SEARCH_INIT_STRUCTS_FAIL;
        }
        if ((g->open_intlist[1] = cr_list()) == NULL)
        {
            //error_val = GRAPH_SEARCH_INIT_STRUCTS_FAIL;
            print_errorv(GRAPH_SEARCH_INIT_STRUCTS_FAIL);
            return GRAPH_SEARCH_INIT_STRUCTS_FAIL;
        }
    }
    v_update_loop(g->visited,get_index_size(g->inIndex));
    if(type!=BFS){
        empty_list(g->open_intlist[0]);
        empty_list(g->open_intlist[1]);
    }
    else{
         st_empty_list(g->open_list);
    }
    if(g->type==DYNAMIC){
        if(!CC_same_component_2(g->ccindex,from,to)){
            return GRAPH_SEARCH_PATH_NOT_FOUND;
        }
    }
    else if(g->type==STATIC){
        if (findNodeStronglyConnectedComponentID(g->sccs, from) == findNodeStronglyConnectedComponentID(g->sccs, to))
        {   // nodes belong in same component
            return estimateShortestPathStronglyConnectedComponents(g->sccs, g, from, to, g->open_intlist, g->visited);
        }
        else if (isReachableGrailIndex(g->grail,g->sccs, from, to) == 0)
        {   // grail told us that there is no path
            return GRAPH_SEARCH_PATH_NOT_FOUND;
        }
        else    // MAYBE
        {   // we are not sure; we must search
            return bidirectional_bfs_grail(g, from, to, g->open_intlist, g->visited);
        }
    }
    (type == BFS ? (return_value=bfs(g, from, to)) : (return_value=bidirectional_bfs(g, from, to, g->open_intlist, g->visited,0)));
    return return_value;
}

int gFindShortestPath_t(pGraph g, graphNode from, graphNode to, phead *lists, pvis visited, uint32_t version,int *queries,int *update_queries)
{
    int return_value;
    if (g == NULL)
    {
        //error_val = GRAPH_NULL_POINTER_PROVIDED;
        print_errorv(GRAPH_NULL_POINTER_PROVIDED);
        return GRAPH_NULL_POINTER_PROVIDED;
    }
    if (from == to)
    {
        //error_val = OK_SUCCESS;
        return OK_SUCCESS;
    }
    v_update_loop(visited,get_index_size(g->inIndex));
    empty_list(lists[0]);
    empty_list(lists[1]);
    if(g->type==DYNAMIC){
        if(!CC_same_component_2_t(g->ccindex,from,to,version,queries,update_queries)){
            return GRAPH_SEARCH_PATH_NOT_FOUND;
        }
        v_update_loop(visited,get_index_size(g->inIndex));
    }
    else if(g->type==STATIC){
        //TODO change to thread safe functions
        if (findNodeStronglyConnectedComponentID(g->sccs, from) == findNodeStronglyConnectedComponentID(g->sccs, to))
        {   // nodes belong in same component
            return estimateShortestPathStronglyConnectedComponents(g->sccs, g, from, to, lists, visited);
        }
        else if (isReachableGrailIndex(g->grail,g->sccs, from, to) == 0)
        {   // grail told us that there is no path
            return GRAPH_SEARCH_PATH_NOT_FOUND;
        }
        else    // MAYBE
        {   // we are not sure; we must search
            return bidirectional_bfs_grail(g, from, to, lists, visited);
        }
    }
    return_value=bidirectional_bfs(g, from, to, lists, visited,version);
    return return_value;
}

int bfs(pGraph g, graphNode from, graphNode to)
{
    int i, return_value, temp_node_tag;
    graphNode temp_node;
    pBuffer temp_buffer;
    ptr buffer_ptr_to_listnode;
    plnode listnode;
    // TODO change 1 to 0 with greater hash size to see if it adjust better in greater amount of data
    if ((return_value = st_insert_back(g->open_list, from, 0 )) != OK_SUCCESS)
    {
        //// update error_value changed by destroy_<>() functions
        //error_val = return_value;
        print_errorv(GRAPH_BFS_FAIL_INSERT_TO_QUEUE);
        return GRAPH_BFS_FAIL_INSERT_TO_QUEUE;
    }
    while(st_get_size(g->open_list) > 0)
    {
        if ((temp_node = st_peek(g->open_list)) < 0)
        {
            //error_val = temp_node;
            print_errorv(GRAPH_BFS_FAIL_QUEUE_PEEK);
            return GRAPH_BFS_FAIL_QUEUE_PEEK;
        }
        if ((temp_node_tag = st_get_tag(g->open_list, temp_node)) < 0)
        {
            //error_val = temp_node_tag;
            print_errorv(GRAPH_BFS_FAIL_CHECK_BFS_TAG);
            return GRAPH_BFS_FAIL_CHECK_BFS_TAG;
        }
        if (temp_node == to)
        {    // target node found
            //error_val = OK_SUCCESS;
            return temp_node_tag;
        }
        if ((return_value = st_pop_front(g->open_list)) < 0)
        {
            //error_val = return_value;
            print_errorv(GRAPH_BFS_FAIL_QUEUE_POP);
            return GRAPH_BFS_FAIL_QUEUE_POP;
        }
        // check if current node is already visited
        return_value =v_visited(g->visited, temp_node);
        if (return_value < 0)
        {
            //error_val = return_value;
            print_errorv(GRAPH_BFS_FAIL_CHECK_VISITED);
            return GRAPH_BFS_FAIL_CHECK_VISITED;
        }
        if (return_value > 0)
            continue;    // node already visited
        // if return_value == 0, then the node hasn't been visited yet, so we visit & expand him
        if ((return_value = v_mark(g->visited, temp_node, 0, 0)) < 0)
        {
            //error_val = return_value;
            print_errorv(GRAPH_BFS_FAIL_CHECK_VISITED);
            return GRAPH_BFS_FAIL_CHECK_VISITED;
        }
        // add to number of total additions the above addition
        buffer_ptr_to_listnode = getListHead(g->outIndex, temp_node);
        if (buffer_ptr_to_listnode == -1)
        {   // node has no neighbors
            continue;
        }
        else if (buffer_ptr_to_listnode < 0)
        {   // an error occurred
            //error_val = return_value;
            print_errorv(GRAPH_BFS_FAIL_GET_LIST_HEAD);
            return GRAPH_BFS_FAIL_GET_LISTNODE;
        }
        if ((temp_buffer = return_buffer(g->outIndex)) == NULL)
        {
            //error_val = return_value;
            print_errorv(GRAPH_BFS_FAIL_GET_BUFFER);
            return GRAPH_BFS_FAIL_GET_BUFFER;
        }
        if ((listnode = getListNode(temp_buffer, buffer_ptr_to_listnode)) == NULL)
        {
            //error_val = return_value;
            print_errorv(GRAPH_BFS_FAIL_GET_LISTNODE);
            return GRAPH_BFS_FAIL_GET_LISTNODE;
        }
        i = 0;
        while (listnode->neighbor[i] != -1)
        {
            return_value = v_visited(g->visited, listnode->neighbor[i]);
            if (return_value < 0)
            {
                //error_val = return_value;
                print_errorv(GRAPH_BFS_FAIL_CHECK_VISITED);
                return GRAPH_BFS_FAIL_CHECK_VISITED;
            }
            if (!return_value)
            {    // if this node hasn't been visited yet
                if ((return_value = st_insert_back(g->open_list, listnode->neighbor[i], temp_node_tag+1)) != OK_SUCCESS)
                {
                    //error_val = return_value;
                    print_errorv(GRAPH_BFS_FAIL_INSERT_TO_QUEUE);
                    return GRAPH_BFS_FAIL_INSERT_TO_QUEUE;
                }
            }
            i++;
            if (i == N)
            {
                if (listnode->nextListNode == -1)   // if there are no more neighbors, break
                    break;
                if ((listnode = getListNode(temp_buffer, listnode->nextListNode)) == NULL)
                {
                    //error_val = return_value;
                    print_errorv(GRAPH_BFS_FAIL_GET_LISTNODE);
                    return GRAPH_BFS_FAIL_GET_LISTNODE;
                }
                i = 0;
            }
        }
    }
    // check if control exited the loop because of an error instead of just empty list
    if ((return_value = st_get_size(g->open_list)) < 0)
    {
        //error_val = return_value;
        print_errorv(GRAPH_BFS_FAIL_GET_QUEUE_SIZE);
        return GRAPH_BFS_FAIL_GET_QUEUE_SIZE;
    }
    //error_val = OK_SUCCESS;
    return GRAPH_SEARCH_PATH_NOT_FOUND;
}

int bidirectional_bfs(pGraph g, graphNode from, graphNode to, phead *open_list, pvis visited,uint32_t version)
{    // 0 is for out-Index, 1 is for in-Index
    int i, n, return_value, path_length[2], grandchildren[2], number_of_nodes, current_bfs, k, edges;
    uint32_t current_neighbor;
    graphNode temp_node;
    pBuffer temp_buffer[2];
    ptr buffer_ptr_to_listnode;
    plnode listnode;
    if (insert_back(open_list[0], from) != OK_SUCCESS)
    {
        //print_error();
        //error_val = GRAPH_BFS_FAIL_INSERT_TO_QUEUE;
        print_errorv(GRAPH_BFS_FAIL_INSERT_TO_QUEUE);
        return GRAPH_BFS_FAIL_INSERT_TO_QUEUE;
    }
    if (v_mark(visited, from, 0, VISITED) < 0)
    {
        //print_error();
        //error_val = GRAPH_BFS_FAIL_INSERT_TO_HASHTABLE;
        print_errorv(GRAPH_BFS_FAIL_INSERT_TO_HASHTABLE);
        return GRAPH_BFS_FAIL_INSERT_TO_HASHTABLE;
    }
    if (insert_back(open_list[1], to) != OK_SUCCESS)
    {
        //print_error();
        //error_val = GRAPH_BFS_FAIL_INSERT_TO_QUEUE;
        print_errorv(GRAPH_BFS_FAIL_INSERT_TO_QUEUE);
        return GRAPH_BFS_FAIL_INSERT_TO_QUEUE;
    }
    if (v_mark(visited, to, 1, VISITED) < 0)
    {
        //print_error();
        //error_val = GRAPH_BFS_FAIL_INSERT_TO_HASHTABLE;
        print_errorv(GRAPH_BFS_FAIL_INSERT_TO_HASHTABLE);
        return GRAPH_BFS_FAIL_INSERT_TO_HASHTABLE;
    }
    path_length[0] = 0;
    path_length[1] = 0;
    grandchildren[0] = 0;
    grandchildren[1] = 0;
    current_bfs = 1;
    if ((temp_buffer[0] = return_buffer(g->outIndex )) == NULL)
    {
        //print_error();
        //error_val = GRAPH_BFS_FAIL_GET_BUFFER;
        print_errorv(GRAPH_BFS_FAIL_GET_BUFFER);
        return GRAPH_BFS_FAIL_GET_BUFFER;
    }
    if ((temp_buffer[1] = return_buffer(g->inIndex )) == NULL)
    {
        //print_error();
        //error_val = GRAPH_BFS_FAIL_GET_BUFFER;
        print_errorv(GRAPH_BFS_FAIL_GET_BUFFER);
        return GRAPH_BFS_FAIL_GET_BUFFER;
    }

    while(get_size(open_list[0]) > 0 && get_size(open_list[1]) > 0)
    {
        // "<=" used instead of "<", so that if in first bfs only 1 child node is added, then the other bfs will run and push its own starting node.
        // This prevents the extreme case where every node in the path has only 1 child, and if "<" was used only the first bfs would expand nodes continuously,
        // while the other bfs wouldn't have entered its first node in "visited", so the two bfss wouldn't be able to meet
        if ((get_size(open_list[1-current_bfs]) + grandchildren[1-current_bfs]) <= (get_size(open_list[current_bfs]) + grandchildren[current_bfs]))
            current_bfs = 1-current_bfs;
        grandchildren[current_bfs] = 0;
        ++path_length[current_bfs];
        if ((number_of_nodes = get_size(open_list[current_bfs])) < 0)
        {
            //print_error();
            //error_val = GRAPH_BFS_FAIL_GET_QUEUE_SIZE;
            print_errorv(GRAPH_BFS_FAIL_GET_QUEUE_SIZE);
            return GRAPH_BFS_FAIL_GET_QUEUE_SIZE;
        }
        for (n = 0 ; n < number_of_nodes ; ++n)
        {
            if ((temp_node = peek(open_list[current_bfs])) < 0)
            {
                //print_error();
                //error_val = GRAPH_BFS_FAIL_QUEUE_PEEK;
                print_errorv(GRAPH_BFS_FAIL_QUEUE_PEEK);
                return GRAPH_BFS_FAIL_QUEUE_PEEK;
            }
            if (pop_front(open_list[current_bfs]) < 0)
            {
                //print_error();
                //error_val = GRAPH_BFS_FAIL_QUEUE_POP;
                print_errorv(GRAPH_BFS_FAIL_QUEUE_POP);
                return GRAPH_BFS_FAIL_QUEUE_POP;
            }
            return_value =v_set_expanded(visited, temp_node, EXPANDED);
            buffer_ptr_to_listnode = getListHead((current_bfs == 0 ? g->outIndex : g->inIndex), temp_node);
            if (buffer_ptr_to_listnode == -1)
            {   // node has no neighbors
                continue;
            }
            else if (buffer_ptr_to_listnode < 0)
            {   // an error occurred
                //print_error();
                //error_val = GRAPH_BFS_FAIL_GET_LIST_HEAD;
                print_errorv(GRAPH_BFS_FAIL_GET_LIST_HEAD);
                return GRAPH_BFS_FAIL_GET_LIST_HEAD;
            }
            if ((listnode = getListNode(temp_buffer[current_bfs], buffer_ptr_to_listnode)) == NULL)
            {
                //print_error();
                //error_val = GRAPH_BFS_FAIL_GET_LISTNODE;
                print_errorv(GRAPH_BFS_FAIL_GET_LISTNODE);
                return GRAPH_BFS_FAIL_GET_LISTNODE;
            }
            i = 0;
            edges = get_node_number_of_edges((current_bfs == 0 ? g->outIndex : g->inIndex),temp_node);
            for(k = 0 ; k < edges ; k++)
            {
                if(listnode->edgeProperty[i]<=version){
                    current_neighbor = listnode->neighbor[i];
                    // check if the two nodes (from-to) are directly connected
                    if (path_length[current_bfs] == 1 && current_bfs == 0 && current_neighbor == to)
                    {    // the two nodes are direct neighbors, so the path is 1
                        return 1;
                    }
                    return_value = v_visited(visited, current_neighbor);
                    if (return_value < 0)
                    {
                        //print_error();
                        //error_val = GRAPH_BFS_FAIL_CHECK_VISITED;
                        print_errorv(GRAPH_BFS_FAIL_CHECK_VISITED);
                        return GRAPH_BFS_FAIL_CHECK_VISITED;
                    }
                    else if (!return_value)
                    {    // if this node hasn't been visited yet, insert it to the list
                        if (insert_back(open_list[current_bfs], current_neighbor) != OK_SUCCESS)
                        {
                            //print_error();
                            //error_val = GRAPH_BFS_FAIL_INSERT_TO_QUEUE;
                            print_errorv(GRAPH_BFS_FAIL_INSERT_TO_QUEUE);
                            return GRAPH_BFS_FAIL_INSERT_TO_QUEUE;
                        }
                        if (v_mark(visited, current_neighbor, current_bfs, VISITED) < 0)
                        {
                            //print_error();
                            //error_val = GRAPH_BFS_FAIL_INSERT_TO_HASHTABLE;
                            print_errorv(GRAPH_BFS_FAIL_INSERT_TO_HASHTABLE);
                            return GRAPH_BFS_FAIL_INSERT_TO_HASHTABLE;
                        }
                        grandchildren[current_bfs]+=*(get_node_number_of_edges_2((current_bfs == 0 ? g->outIndex : g->inIndex), current_neighbor));
                    }
                    else if (return_value > 0)
                    {
                        return_value = v_ret_tag(visited, current_neighbor);
                        if (return_value < 0)
                        {
                            //print_error();
                            //error_val = GRAPH_BFS_FAIL_CHECK_BFS_TAG;
                            print_errorv(GRAPH_BFS_FAIL_CHECK_BFS_TAG);
                            return GRAPH_BFS_FAIL_CHECK_BFS_TAG;
                        }
                        if (return_value == 1-current_bfs)
                        {    // if the tag of the neighbor that's already on the 'visited' list is the other bfs' tag
                            // then the two bfss have just met so a path has been found
                            if (v_ret_expanded(visited, current_neighbor) == VISITED)
                                path_length[1-current_bfs]++;
                            return path_length[0] + path_length[1] - 1;
                        }
                        // if return value (=tag) == current_bfs, then the node has already been visited by this bfs, so it doesn't enter the open list again
                    }
                }
                i++;
                if (i == N)
                {
                    if (listnode->nextListNode == -1)   // if there are no more neighbors, break
                        break;
                    if ((listnode = getListNode(temp_buffer[current_bfs], listnode->nextListNode)) == NULL)
                    {
                        //print_error();
                        //error_val = GRAPH_BFS_FAIL_GET_LISTNODE;
                        print_errorv(GRAPH_BFS_FAIL_GET_LISTNODE);
                        return GRAPH_BFS_FAIL_GET_LISTNODE;
                    }
                    i = 0;
                }
            }
        }
    }
    return GRAPH_SEARCH_PATH_NOT_FOUND;
}

int bidirectional_bfs_inside_component(pSCC components, pGraph g, uint32_t from, uint32_t to, uint32_t component_id, phead open_list[2], pvis visited)
{   // 0 is for out-Index, 1 is for in-Index
    int i, n, return_value, path_length[2], grandchildren[2], number_of_nodes, current_bfs, k, edges;
    uint32_t current_neighbor;
    graphNode temp_node;
    pBuffer temp_buffer;
    ptr buffer_ptr_to_listnode;
    plnode listnode;
    if (insert_back(open_list[0], from) != OK_SUCCESS)
    {
        //print_error();
        //error_val = GRAPH_BFS_FAIL_INSERT_TO_QUEUE;
        print_errorv(GRAPH_BFS_FAIL_INSERT_TO_QUEUE);
        return GRAPH_BFS_FAIL_INSERT_TO_QUEUE;
    }
    if (v_mark(visited, from, 0, VISITED) < 0)
    {
        //print_error();
        //error_val = GRAPH_BFS_FAIL_INSERT_TO_HASHTABLE;
        print_errorv(GRAPH_BFS_FAIL_INSERT_TO_HASHTABLE);
        return GRAPH_BFS_FAIL_INSERT_TO_HASHTABLE;
    }
    if (insert_back(open_list[1], to) != OK_SUCCESS)
    {
        //print_error();
        //error_val = GRAPH_BFS_FAIL_INSERT_TO_QUEUE;
        print_errorv(GRAPH_BFS_FAIL_INSERT_TO_QUEUE);
        return GRAPH_BFS_FAIL_INSERT_TO_QUEUE;
    }
    if (v_mark(visited, to, 1, VISITED) < 0)
    {
        //print_error();
        //error_val = GRAPH_BFS_FAIL_INSERT_TO_HASHTABLE;
        print_errorv(GRAPH_BFS_FAIL_INSERT_TO_HASHTABLE);
        return GRAPH_BFS_FAIL_INSERT_TO_HASHTABLE;
    }
    path_length[0] = 0;
    path_length[1] = 0;
    grandchildren[0] = 0;
    grandchildren[1] = 0;
    current_bfs = 1;
    while(get_size(open_list[0]) > 0 && get_size(open_list[1]) > 0)
    {
        // "<=" used instead of "<", so that if in first bfs only 1 child node is added, then the other bfs will run and push its own starting node.
        // This prevents the extreme case where every node in the path has only 1 child, and if "<" was used only the first bfs would expand nodes continuously,
        // while the other bfs wouldn't have entered its first node in "visited", so the two bfss wouldn't be able to meet
        if ((get_size(open_list[1-current_bfs]) + grandchildren[1-current_bfs]) <= (get_size(open_list[current_bfs]) + grandchildren[current_bfs]))
            current_bfs = 1-current_bfs;
        grandchildren[current_bfs] = 0;
        ++path_length[current_bfs];
        if ((number_of_nodes = get_size(open_list[current_bfs])) < 0)
        {
            //print_error();
            //error_val = GRAPH_BFS_FAIL_GET_QUEUE_SIZE;
            print_errorv(GRAPH_BFS_FAIL_GET_QUEUE_SIZE);
            return GRAPH_BFS_FAIL_GET_QUEUE_SIZE;
        }
        for (n = 0 ; n < number_of_nodes ; ++n)
        {
            if ((temp_node = peek(open_list[current_bfs])) < 0)
            {
                //print_error();
                //error_val = GRAPH_BFS_FAIL_QUEUE_PEEK;
                print_errorv(GRAPH_BFS_FAIL_QUEUE_PEEK);
                return GRAPH_BFS_FAIL_QUEUE_PEEK;
            }
            if (pop_front(open_list[current_bfs]) < 0)
            {
                //print_error();
                //error_val = GRAPH_BFS_FAIL_QUEUE_POP;
                print_errorv(GRAPH_BFS_FAIL_QUEUE_POP);
                return GRAPH_BFS_FAIL_QUEUE_POP;
            }
            return_value =v_set_expanded(visited, temp_node, EXPANDED);
            buffer_ptr_to_listnode = getListHead((current_bfs == 0 ? g->outIndex : g->inIndex), temp_node);
            if (buffer_ptr_to_listnode == -1)
            {   // node has no neighbors
                continue;
            }
            else if (buffer_ptr_to_listnode < 0)
            {   // an error occurred
                //print_error();
                //error_val = GRAPH_BFS_FAIL_GET_LIST_HEAD;
                print_errorv(GRAPH_BFS_FAIL_GET_LIST_HEAD);
                return GRAPH_BFS_FAIL_GET_LIST_HEAD;
            }
            if ((temp_buffer = return_buffer((current_bfs == 0 ? g->outIndex : g->inIndex))) == NULL)
            {
                //print_error();
                //error_val = GRAPH_BFS_FAIL_GET_BUFFER;
                print_errorv(GRAPH_BFS_FAIL_GET_BUFFER);
                return GRAPH_BFS_FAIL_GET_BUFFER;
            }
            if ((listnode = getListNode(temp_buffer, buffer_ptr_to_listnode)) == NULL)
            {
                //print_error();
                //error_val = GRAPH_BFS_FAIL_GET_LISTNODE;
                print_errorv(GRAPH_BFS_FAIL_GET_LISTNODE);
                return GRAPH_BFS_FAIL_GET_LISTNODE;
            }
            i = 0;
            edges = get_node_number_of_edges((current_bfs == 0 ? g->outIndex : g->inIndex),temp_node);
            for(k = 0 ; k < edges ; k++)
            {
                current_neighbor = listnode->neighbor[i];
                if (findNodeStronglyConnectedComponentID(components, current_neighbor) == component_id)
                {
                    // check if the two nodes (from-to) are directly connected
                    if (path_length[current_bfs] == 1 && current_bfs == 0 && current_neighbor == to)
                    {    // the two nodes are direct neighbors, so the path is 1
                        return 1;
                    }
                    return_value = v_visited(visited, current_neighbor);
                    if (return_value < 0)
                    {
                        //print_error();
                        //error_val = GRAPH_BFS_FAIL_CHECK_VISITED;
                        print_errorv(GRAPH_BFS_FAIL_CHECK_VISITED);
                        return GRAPH_BFS_FAIL_CHECK_VISITED;
                    }
                    else if (!return_value)
                    {    // if this node hasn't been visited yet, insert it to the list
                        if (insert_back(open_list[current_bfs], current_neighbor) != OK_SUCCESS)
                        {
                            //print_error();
                            //error_val = GRAPH_BFS_FAIL_INSERT_TO_QUEUE;
                            print_errorv(GRAPH_BFS_FAIL_INSERT_TO_QUEUE);
                            return GRAPH_BFS_FAIL_INSERT_TO_QUEUE;
                        }
                        if (v_mark(visited, current_neighbor, current_bfs, VISITED) < 0)
                        {
                            //print_error();
                            //error_val = GRAPH_BFS_FAIL_INSERT_TO_HASHTABLE;
                            print_errorv(GRAPH_BFS_FAIL_INSERT_TO_HASHTABLE);
                            return GRAPH_BFS_FAIL_INSERT_TO_HASHTABLE;
                        }
                        grandchildren[current_bfs] += get_node_number_of_edges((current_bfs == 0 ? g->outIndex : g->inIndex), current_neighbor);
                    }
                    else if (return_value > 0)
                    {
                        return_value = v_ret_tag(visited, current_neighbor);
                        if (return_value < 0)
                        {
                            //print_error();
                            //error_val = GRAPH_BFS_FAIL_CHECK_BFS_TAG;
                            print_errorv(GRAPH_BFS_FAIL_CHECK_BFS_TAG);
                            return GRAPH_BFS_FAIL_CHECK_BFS_TAG;
                        }
                        if (return_value == 1-current_bfs)
                        {    // if the tag of the neighbor that's already on the 'visited' list is the other bfs' tag
                            // then the two bfss have just met so a path has been found
                            if (v_ret_expanded(visited, current_neighbor) == VISITED)
                                path_length[1-current_bfs]++;
                            return path_length[0] + path_length[1] - 1;
                        }
                        // if return value (=tag) == current_bfs, then the node has already been visited by this bfs, so it doesn't enter the open list again
                    }
                }
                i++;
                if (i == N)
                {
                    if (listnode->nextListNode == -1)   // if there are no more neighbors, break
                        break;
                    if ((listnode = getListNode(temp_buffer, listnode->nextListNode)) == NULL)
                    {
                        //print_error();
                        //error_val = GRAPH_BFS_FAIL_GET_LISTNODE;
                        print_errorv(GRAPH_BFS_FAIL_GET_LISTNODE);
                        return GRAPH_BFS_FAIL_GET_LISTNODE;
                    }
                    i = 0;
                }
            }
        }
    }
    return GRAPH_SEARCH_PATH_NOT_FOUND;
}
/*
int bidirectional_bfs_grail(pGraph g, graphNode from, graphNode to)
{    // 0 is for out-Index, 1 is for in-Index
    static int i, n, return_value, path_length[2], grandchildren[2], number_of_nodes, current;
    static char path_found ;
    static graphNode temp_node;
    static pBuffer temp_buffer;
    static ptr buffer_ptr_to_listnode;
    static plnode listnode;
    path_found=0;
    if ((return_value = insert_back(g->open_intlist[0], from)) != OK_SUCCESS)
    {
        error_val = return_value;
        return return_value;
    }
    if ((return_value = v_mark(g->visited, from, 0, VISITED)) < 0)
    {
        error_val = return_value;
        return return_value;
    }
    if ((return_value = insert_back(g->open_intlist[1], to)) != OK_SUCCESS)
    {
        error_val = return_value;
        return return_value;
    }
    if ((return_value = v_mark(g->visited, to, 1, VISITED)) < 0)
    {
        error_val = return_value;
        return return_value;
    }
    path_length[0] = 0;
    path_length[1] = 0;
    grandchildren[0] = 0;
    grandchildren[1] = 0;
    current = 1;
    while(get_size(g->open_intlist[0]) > 0 && get_size(g->open_intlist[1]) > 0)
    {
        // "<=" used instead of "<", so that if in first bfs only 1 child node is added, then the other bfs will run and push its own starting node.
        // This prevents the extreme case where every node in the path has only 1 child, and if "<" was used only the first bfs would expand nodes continuously,
        // while the other bfs wouldn't have entered its first node in "visited", so the two bfss wouldn't be able to meet
        if ((get_size(g->open_intlist[1-current]) + grandchildren[1-current]) <= (get_size(g->open_intlist[current]) + grandchildren[current]))
            current = 1-current;
        grandchildren[current] = 0;
        path_length[current]++;
        if ((number_of_nodes = get_size(g->open_intlist[current])) < 0)
        {
            error_val = number_of_nodes;
            return number_of_nodes;
        }
        for (n = 0 ; n < number_of_nodes ; ++n)
        {
            if ((temp_node = peek(g->open_intlist[current])) < 0)
            {
                error_val = temp_node;
                return temp_node;
            }
            if ((return_value = pop_front(g->open_intlist[current])) < 0)
            {
                error_val = return_value;
                return return_value;
            }
            //check if we found a connection
            // check if the two nodes (from-to) are directly connected
            if (path_length[current] == 1 && current == 0 && temp_node == to)
            {    // the two nodes are direct neighbors, so the path is 1
                return 1;
            }
            return_value = v_visited(g->visited, temp_node);
            if (return_value < 0)
            {
                error_val = return_value;
                return return_value;
            }
            //else if (!return_value)
            //{   // node not visited yet
            //
            //}
            assert(!return_value);  //we can't expand a node not visited yet
            // return_value > 0
            //node already visited
            return_value = v_ret_tag(g->visited, listnode->neighbor[i]);
            if (return_value < 0)
            {
                error_val = return_value;
                //error
                return return_value;
            }
            if (return_value == 1-current)
            {    // if the tag of the neighbor that's already on the 'visited' list is the other bfs' tag
                // then the two bfss have just met so a path has been found
                if ((return_value = v_ret_expanded(g->visited, listnode->neighbor[i])) < 0)
                {
                    error_val = return_value;
                    //error
                    return return_value;
                }
                if (return_value == VISITED)    // TODO: THIS SHOULD NEVER HAPPEN
                    path_length[1-current]++;
                path_found = 1;
                break;
            }
            if ((return_value =v_set_expanded(g->visited, temp_node, EXPANDED)) < 0)
            {
                error_val = return_value;
                //error
                return return_value;
            }
            buffer_ptr_to_listnode = getListHead((current == 0 ? g->outIndex : g->inIndex), temp_node);
            if (buffer_ptr_to_listnode == -1)
            {   // node has no neighbors
                continue;
            }
            else if (buffer_ptr_to_listnode < 0)
            {   // an error occurred
                error_val = buffer_ptr_to_listnode;
                //error
                return buffer_ptr_to_listnode;
            }
            if ((temp_buffer = return_buffer((current == 0 ? g->outIndex : g->inIndex))) == NULL)
            {
                return_value = error_val;
                error_val = return_value;
                //error
                return return_value;
            }
            if ((listnode = getListNode(temp_buffer, buffer_ptr_to_listnode)) == NULL)
            {
                return_value = error_val;
                error_val = return_value;
                //error
                return return_value;
            }
            i = 0;
            while (listnode->neighbor[i] != -1)
            {
                return_value = v_visited(g->visited, listnode->neighbor[i]);
                if (return_value < 0)
                {
                    error_val = return_value;
                    //error
                    return return_value;
                }
                else if (!return_value)
                {    // if this node hasn't been visited yet, insert it to the list
                    if ((return_value = insert_back(g->open_intlist[current], listnode->neighbor[i])) != OK_SUCCESS)
                    {
                        error_val = return_value;
                        //error
                        return return_value;
                    }
                    if ((return_value = v_mark(g->visited, listnode->neighbor[i], current, VISITED)) < 0)
                    {
                        error_val = return_value;
                        //error
                        return return_value;
                    }
                    grandchildren[current] += get_node_number_of_edges((current == 0 ? g->outIndex : g->inIndex), listnode->neighbor[i]);
                }
                else if (return_value > 0)
                {
                    //
                }
                i++;
                if (i == N)
                {
                    if (listnode->nextListNode == -1)   // if there are no more neighbors, break
                        break;
                    if ((listnode = getListNode(temp_buffer, listnode->nextListNode)) == NULL)
                    {
                        return_value = error_val;
                        error_val = return_value;
                        //error
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
    if (path_found)
        return path_length[0] + path_length[1] - 1;
    else
        return GRAPH_SEARCH_PATH_NOT_FOUND;
}
*/
int bidirectional_bfs_grail(pGraph g, graphNode from, graphNode to, phead open_list[2], pvis visited)
{    // 0 is for out-Index, 1 is for in-Index
    int i, n, return_value, path_length[2], grandchildren[2], number_of_nodes, current_bfs, scc_from, scc_to, k, edges;
    uint32_t current_neighbor;
    graphNode temp_node;
    pBuffer temp_buffer;
    ptr buffer_ptr_to_listnode;
    plnode listnode;
    if ((scc_from = findNodeStronglyConnectedComponentID(g->sccs, from)) < 0)
    {
        //print_error();
        //error_val = GRAPH_BFS_FAIL_GET_SCC_ID;
        print_errorv(GRAPH_BFS_FAIL_GET_SCC_ID);
        return GRAPH_BFS_FAIL_GET_SCC_ID;
    }
    if ((scc_to = findNodeStronglyConnectedComponentID(g->sccs, to)) < 0)
    {
        //print_error();
        //error_val = GRAPH_BFS_FAIL_GET_SCC_ID;
        print_errorv(GRAPH_BFS_FAIL_GET_SCC_ID);
        return GRAPH_BFS_FAIL_GET_SCC_ID;
    }
    if (insert_back(open_list[0], from) != OK_SUCCESS)
    {
        //print_error();
        //error_val = GRAPH_BFS_FAIL_INSERT_TO_QUEUE;
        print_errorv(GRAPH_BFS_FAIL_INSERT_TO_QUEUE);
        return GRAPH_BFS_FAIL_INSERT_TO_QUEUE;
    }
    if (v_mark(visited, from, 0, VISITED) < 0)
    {
        //print_error();
        //error_val = GRAPH_BFS_FAIL_INSERT_TO_HASHTABLE;
        print_errorv(GRAPH_BFS_FAIL_INSERT_TO_HASHTABLE);
        return GRAPH_BFS_FAIL_INSERT_TO_HASHTABLE;
    }
    if (insert_back(open_list[1], to) != OK_SUCCESS)
    {
        //print_error();
        //error_val = GRAPH_BFS_FAIL_INSERT_TO_QUEUE;
        print_errorv(GRAPH_BFS_FAIL_INSERT_TO_QUEUE);
        return GRAPH_BFS_FAIL_INSERT_TO_QUEUE;
    }
    if (v_mark(visited, to, 1, VISITED) < 0)
    {
        //print_error();
        //error_val = GRAPH_BFS_FAIL_INSERT_TO_HASHTABLE;
        print_errorv(GRAPH_BFS_FAIL_INSERT_TO_HASHTABLE);
        return GRAPH_BFS_FAIL_INSERT_TO_HASHTABLE;
    }
    path_length[0] = 0;
    path_length[1] = 0;
    grandchildren[0] = 0;
    grandchildren[1] = 0;
    current_bfs = 1;
    while(get_size(open_list[0]) > 0 && get_size(open_list[1]) > 0)
    {
        // "<=" used instead of "<", so that if in first bfs only 1 child node is added, then the other bfs will run and push its own starting node.
        // This prevents the extreme case where every node in the path has only 1 child, and if "<" was used only the first bfs would expand nodes continuously,
        // while the other bfs wouldn't have entered its first node in "visited", so the two bfss wouldn't be able to meet
        if ((get_size(open_list[1-current_bfs]) + grandchildren[1-current_bfs]) <= (get_size(open_list[current_bfs]) + grandchildren[current_bfs]))
            current_bfs = 1-current_bfs;
        grandchildren[current_bfs] = 0;
        ++path_length[current_bfs];
        if ((number_of_nodes = get_size(open_list[current_bfs])) < 0)
        {
            //print_error();
            //error_val = GRAPH_BFS_FAIL_GET_QUEUE_SIZE;
            print_errorv(GRAPH_BFS_FAIL_GET_QUEUE_SIZE);
            return GRAPH_BFS_FAIL_GET_QUEUE_SIZE;
        }
        for (n = 0 ; n < number_of_nodes ; ++n)
        {
            if ((temp_node = peek(open_list[current_bfs])) < 0)
            {
                //print_error();
                //error_val = GRAPH_BFS_FAIL_QUEUE_PEEK;
                print_errorv(GRAPH_BFS_FAIL_QUEUE_PEEK);
                return GRAPH_BFS_FAIL_QUEUE_PEEK;
            }
            if (pop_front(open_list[current_bfs]) < 0)
            {
                //print_error();
                //error_val = GRAPH_BFS_FAIL_QUEUE_POP;
                print_errorv(GRAPH_BFS_FAIL_QUEUE_POP);
                return GRAPH_BFS_FAIL_QUEUE_POP;
            }
            v_set_expanded(visited, temp_node, EXPANDED);
            buffer_ptr_to_listnode = getListHead((current_bfs == 0 ? g->outIndex : g->inIndex), temp_node);
            if (buffer_ptr_to_listnode == -1)
            {   // node has no neighbors
                continue;
            }
            else if (buffer_ptr_to_listnode < 0)
            {   // an error occurred
                //print_error();
                //error_val = GRAPH_BFS_FAIL_GET_LIST_HEAD;
                print_errorv(GRAPH_BFS_FAIL_GET_LIST_HEAD);
                return GRAPH_BFS_FAIL_GET_LIST_HEAD;
            }
            if ((temp_buffer = return_buffer((current_bfs == 0 ? g->outIndex : g->inIndex))) == NULL)
            {
                //print_error();
                //error_val = GRAPH_BFS_FAIL_GET_BUFFER;
                print_errorv(GRAPH_BFS_FAIL_GET_BUFFER);
                return GRAPH_BFS_FAIL_GET_BUFFER;
            }
            if ((listnode = getListNode(temp_buffer, buffer_ptr_to_listnode)) == NULL)
            {
                //print_error();
                //error_val = GRAPH_BFS_FAIL_GET_LISTNODE;
                print_errorv(GRAPH_BFS_FAIL_GET_LISTNODE);
                return GRAPH_BFS_FAIL_GET_LISTNODE;
            }
            i = 0;
            edges = get_node_number_of_edges((current_bfs == 0 ? g->outIndex : g->inIndex),temp_node);
            for(k = 0 ; k < edges ; k++)
            {
                current_neighbor = listnode->neighbor[i];
                // check if the two nodes (from-to) are directly connected
                if (path_length[current_bfs] == 1 && current_bfs == 0 && current_neighbor == to)
                {    // the two nodes are direct neighbors, so the path is 1
                    return 1;
                }
                return_value = v_visited(visited, current_neighbor);
                if (return_value < 0)
                {
                    //print_error();
                    //error_val = GRAPH_BFS_FAIL_CHECK_VISITED;
                    print_errorv(GRAPH_BFS_FAIL_CHECK_VISITED);
                    return GRAPH_BFS_FAIL_CHECK_VISITED;
                }
                else if (!return_value)
                {    // if this node hasn't been visited yet
                    if (!current_bfs)
                    {
                        if ((findNodeStronglyConnectedComponentID(g->sccs, current_neighbor) == scc_to) || (isReachableGrailIndex(g->grail, g->sccs, current_neighbor, to) != 0))
                        {
                            if (insert_back(open_list[current_bfs], current_neighbor) != OK_SUCCESS)
                            {
                                //print_error();
                                //error_val = GRAPH_BFS_FAIL_INSERT_TO_QUEUE;
                                print_errorv(GRAPH_BFS_FAIL_INSERT_TO_QUEUE);
                                return GRAPH_BFS_FAIL_INSERT_TO_QUEUE;
                            }
                            if (v_mark(visited, current_neighbor, current_bfs, VISITED) < 0)
                            {
                                //print_error();
                                //error_val = GRAPH_BFS_FAIL_INSERT_TO_HASHTABLE;
                                print_errorv(GRAPH_BFS_FAIL_INSERT_TO_HASHTABLE);
                                return GRAPH_BFS_FAIL_INSERT_TO_HASHTABLE;
                            }
                            grandchildren[current_bfs]+=*(get_node_number_of_edges_2(g->outIndex, current_neighbor));
                        }
                    }
                    else    //current_bfs == 1
                    {
                        if ((findNodeStronglyConnectedComponentID(g->sccs, current_neighbor) == scc_from) || (isReachableGrailIndex(g->grail, g->sccs, from, current_neighbor) != 0))
                        {
                            if (insert_back(open_list[current_bfs], current_neighbor) != OK_SUCCESS)
                            {
                                //print_error();
                                //error_val = GRAPH_BFS_FAIL_INSERT_TO_QUEUE;
                                print_errorv(GRAPH_BFS_FAIL_INSERT_TO_QUEUE);
                                return GRAPH_BFS_FAIL_INSERT_TO_QUEUE;
                            }
                            if (v_mark(visited, current_neighbor, current_bfs, VISITED) < 0)
                            {
                                //print_error();
                                //error_val = GRAPH_BFS_FAIL_INSERT_TO_HASHTABLE;
                                print_errorv(GRAPH_BFS_FAIL_INSERT_TO_HASHTABLE);
                                return GRAPH_BFS_FAIL_INSERT_TO_HASHTABLE;
                            }
                            grandchildren[current_bfs]+=*(get_node_number_of_edges_2(g->inIndex, current_neighbor));
                        }
                    }
                }
                else if (return_value > 0)
                {
                    return_value = v_ret_tag(visited, current_neighbor);
                    if (return_value < 0)
                    {
                        //print_error();
                        //error_val = GRAPH_BFS_FAIL_CHECK_BFS_TAG;
                        print_errorv(GRAPH_BFS_FAIL_CHECK_BFS_TAG);
                        return GRAPH_BFS_FAIL_CHECK_BFS_TAG;
                    }
                    if (return_value == 1-current_bfs)
                    {    // if the tag of the neighbor that's already on the 'visited' list is the other bfs' tag
                        // then the two bfss have just met so a path has been found
                        if (v_ret_expanded(visited, current_neighbor) == VISITED)
                            path_length[1-current_bfs]++;
                        return path_length[0] + path_length[1] - 1;
                    }
                    // if return value (=tag) == current_bfs, then the node has already been visited by this bfs, so it doesn't enter the open list again
                }
                i++;
                if (i == N)
                {
                    if (listnode->nextListNode == -1)   // if there are no more neighbors, break
                        break;
                    if ((listnode = getListNode(temp_buffer, listnode->nextListNode)) == NULL)
                    {
                        //print_error();
                        //error_val = GRAPH_BFS_FAIL_GET_LISTNODE;
                        print_errorv(GRAPH_BFS_FAIL_GET_LISTNODE);
                        return GRAPH_BFS_FAIL_GET_LISTNODE;
                    }
                    i = 0;
                }
            }
        }
    }
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
        //print_error();
        return;
    }
    printf("Printing graph...\n");
    for (i = 0 ; i < size ; ++i)
    {
        printf("Node %d\n\tOutcoming:", i);
        if ((buffer_ptr_to_listnode = getListHead(g->outIndex, i)) < 0 && buffer_ptr_to_listnode != -1)
        {
            puts("\n");
            //print_error();
            return;
        }
        if (buffer_ptr_to_listnode != -1)
        {
            if ((temp_buffer = return_buffer(g->outIndex)) == NULL)
            {
                puts("\n");
                //print_error();
                return;
            }
            if ((listnode = getListNode(temp_buffer, buffer_ptr_to_listnode)) == NULL)
            {
                puts("\n");
                //print_error();
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
                        //print_error();
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
            //print_error();
            return;
        }
        if (buffer_ptr_to_listnode != -1)
        {
            if ((temp_buffer = return_buffer(g->inIndex)) == NULL)
            {
                puts("\n");
                //print_error();
                return;
            }
            if ((listnode = getListNode(temp_buffer, buffer_ptr_to_listnode)) == NULL)
            {
                puts("\n");
                //print_error();
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
                        //print_error();
                        return;
                    }
                    j = 0;
                }
            }
        }
        puts("");
    }
}

Index_ptr ret_outIndex(pGraph g){
    return g->outIndex;
}

Index_ptr ret_inIndex(pGraph g){
    return g->inIndex;
}


rcode create_indexes(pGraph g,int type){
    g->type=type;
    if(type==DYNAMIC){
        if ((g->ccindex=CC_create_index(g)) == NULL)
        {
            //error
            return -1;
        }
    }
    else if(type==STATIC){
        if ((g->sccs = estimateStronglyConnectedComponents(g)) == NULL)
        {
            //print_error();
            //error_val = GRAPH_CREATE_STATIC_INDEX_SCCS_FAIL;
            print_errorv(GRAPH_CREATE_STATIC_INDEX_SCCS_FAIL);
            return GRAPH_CREATE_STATIC_INDEX_SCCS_FAIL;
        }
        if (estimateSCCsNeighbors(g->sccs, g) < 0)
        {
            //print_error();
            //error_val = GRAPH_CREATE_STATIC_INDEX_ESTIMATE_SCCS_NEIGHBORS_FAIL;
            print_errorv(GRAPH_CREATE_STATIC_INDEX_ESTIMATE_SCCS_NEIGHBORS_FAIL);
            return GRAPH_CREATE_STATIC_INDEX_ESTIMATE_SCCS_NEIGHBORS_FAIL;
        }
        /*
        printf("SCCs %d\n", get_number_of_components(g->sccs)); //DEBUG
        uint32_t i, sum = 0, temp;
        for (i = 0 ; i < get_number_of_components(g->sccs) ; ++i)
        {
            get_component_neighbors(g->sccs, i, NULL, &temp);
            sum += temp;
        }
        printf("hypergraph edges: %d\n", sum);    //DEBUG
        */
        if ((g->grail = buildGrailIndex(g->sccs,g->open_intlist[0],g->open_intlist[1])) == NULL)
        {
            //print_error();
            //error_val = GRAPH_CREATE_STATIC_INDEX_GRAIL_FAIL;
            print_errorv(GRAPH_CREATE_STATIC_INDEX_GRAIL_FAIL);
            return GRAPH_CREATE_STATIC_INDEX_GRAIL_FAIL;
        }
        empty_list(g->open_intlist[0]);
        empty_list(g->open_intlist[1]);
    }
    //error_val=OK_SUCCESS;
    return OK_SUCCESS;
}

void rebuild(pGraph g){
    if(g->type==DYNAMIC){
        check_rebuild(g->ccindex);
    }
}

void rebuild_t(pGraph g,int* queries,int* update_queries){
    if(g->type==DYNAMIC){
        check_rebuild_t(g->ccindex,queries,update_queries);
    }
}
