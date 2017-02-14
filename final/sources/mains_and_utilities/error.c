#include "../include/error.h"
#include <stdio.h>

int error_val=OK_SUCCESS;

void print_error(){
    switch(error_val){
        case OK_SUCCESS:
            fprintf(stderr, "No error from structures.\n");
            error_val=OK_SUCCESS;
            break;
        case LIST_CR_MALLOC:
            fprintf(stderr, "Error: allocating memory in list creation\n");
            error_val=OK_SUCCESS;
            break;
        case LISTNODE_CR_MALLOC:
            fprintf(stderr, "Error: allocating memory in list node creation\n");
            error_val=OK_SUCCESS;
            break;
        case NULL_LIST:
            fprintf(stderr, "Error: list head called in function with NULL value\n");
            error_val=OK_SUCCESS;
            break;
        case NULL_NODE:
            fprintf(stderr, "Error: list node called in function with NULL value\n");
            error_val=OK_SUCCESS;
            break;
        case HASH_CR_MALLOC:
            fprintf(stderr, "Error: allocating memory in hash table creation\n");
            error_val=OK_SUCCESS;
            break;
        case HASH_BINS_CR_MALLOC:
            fprintf(stderr, "Error: allocating memory in hash table bins creation\n");
            error_val=OK_SUCCESS;
            break;
        case HASH_WRONG_PARMS:
            fprintf(stderr, "Error: wrong parameter in hash table creation:\nvalues must be ----size > 0----type = 1 || 0----\n");
            error_val=OK_SUCCESS;
            break;
        case NULL_HASH:
            fprintf(stderr, "Error: hash table called in function with NULL value\n");
            error_val=OK_SUCCESS;
            break;
        case NODE_MISSING:
            fprintf(stderr, "Error: request of a tag belonging to a non existing node.\n");
            break;
        case BUFFER_CR_MALLOC:
            fprintf(stderr, "Error: allocating struct Buffer\n");
            error_val=OK_SUCCESS;
            break;
        case BUFFER_CR_SPACE_MALLOC:
            fprintf(stderr, "Error: allocating memory space in struct Buffer\n");
            error_val=OK_SUCCESS;
            break;
        case BUFFER_DOUBLE_SIZE:
            fprintf(stderr, "Error: reallocating space and doubling size\n");
            error_val=OK_SUCCESS;
            break;
        case BUFFER_PTR_OUT_BOUNDS:
            fprintf(stderr, "Error: given ptr value exceeds the permitted values\n");
            error_val=OK_SUCCESS;
            break;
        case NULL_BUFFER:
            fprintf(stderr, "Error: Buffer pointer is NULL\n");
            error_val=OK_SUCCESS;
            break;
        case NULL_BUFFER_SPACE:
            fprintf(stderr, "Error: Buffer allocated space pointer points to NULL location\n");
            error_val=OK_SUCCESS;
            break;
        case INDEX_STRUCT_MALLOC_FAIL:
            fprintf(stderr, "Error: allocating space for index struct failed\n");
            error_val=OK_SUCCESS;
            break;
        case INDEX_STRUCT_MALLOC_INNER_FAIL:
            fprintf(stderr, "Error: allocating space in index\n");
            error_val=OK_SUCCESS;
            break;
        case INDEX_CR_WRONG_PARMS:
            fprintf(stderr, "Error: Wrong paramaters in index creation\n");
            error_val=OK_SUCCESS;
            break;
        case INDEX_NULL_HEAD:
            fprintf(stderr, "Error: Null pointer passed in index function \n");
            error_val=OK_SUCCESS;
            break;
        case INDEX_INSERT_NEGATIVE_NODEID :
            fprintf(stderr, "Error: negative id has been passed in index insert\n");
            error_val=OK_SUCCESS;
            break;
        case INDEX_REALLOC_FAIL:
            fprintf(stderr, "Error: reallocating memory in index failed\n");
            error_val=OK_SUCCESS;
            break;
        case INDEX_NODE_ID_OUT_BOUNDS:
            fprintf(stderr, "Error: node id doesn't exist in index (negative or has value than index size)\n");
            error_val=OK_SUCCESS;
            break;
        case INDEX_BUFFER_CR_FAIL:
            fprintf(stderr, "Error: index create function could not create buffer structure\n");
            error_val=OK_SUCCESS;
            break;
        case INDEX_ADD_EDGE_ALLOC_NEW_NODE:
            fprintf(stderr, "Error: index could not allocate memory for a new node\n");
            error_val=OK_SUCCESS;
            break;
        case INDEX_NEIGHBOR_EXISTS:
            fprintf(stderr, "Error: can't add edge to an existing neighbor\n");
            error_val=OK_SUCCESS;
            break;
        case INDEX_GET_LIST_NODE_FAIL:
            fprintf(stderr, "Error: index could not access list node\n");
            error_val=OK_SUCCESS;
            break;
        case INDEX_EDGE_CHECK_FAIL:
            fprintf(stderr, "Error: index could not check if edge exists\n");
            error_val=OK_SUCCESS;
            break;
        case GRAPH_NULL_POINTER_PROVIDED:
            fprintf(stderr, "Error: function called with graph parameter NULL\n");
            error_val=OK_SUCCESS;
            break;
        case GRAPH_CREATION_BASIC_STRUCT_MALLOC_FAIL:
            fprintf(stderr, "Error: malloc for graph structure failed\n");
            error_val=OK_SUCCESS;
            break;
        case GRAPH_CREATION_INDEX_MALLOC_FAIL:
            fprintf(stderr, "Error: malloc for a graph index failed\n");
            error_val=OK_SUCCESS;
            break;
        case GRAPH_CREATION_CR_LIST_FAIL:
            fprintf(stderr, "Error: creating an open_intlist for a graph index failed\n");
            error_val=OK_SUCCESS;
            break;
        case GRAPH_SEARCH_INIT_STRUCTS_FAIL:
            fprintf(stderr, "Error: initializing structs for graph search failed\n");
            error_val=OK_SUCCESS;
            break;
        case GRAPH_HASH_DESTROY_FAIL:
            fprintf(stderr, "Error: An error occurred while destroying graph structs\n");
            error_val=OK_SUCCESS;
            break;
        case VISITED_MALLOC_ERROR:
            fprintf(stderr, "Error: Memory allocation in visited structure failed\n");
            error_val=OK_SUCCESS;
            break;
        case VISITED_REALLOC_FAIL:
            fprintf(stderr, "Error: Memory reallocation in visited structure failed\n");
            error_val=OK_SUCCESS;
            break;
        case VISITED_WRONG_PARAMETERS:
            fprintf(stderr, "Error: Paramaters error in visited function\n");
            error_val=OK_SUCCESS;
            break;
        case CC_MALLOC_FAIL:
            fprintf(stderr, "Error: Memory allocation in CC index failed\n");
            error_val=OK_SUCCESS;
            break;
        case CC_REALLOC_FAIL:
            fprintf(stderr, "Error: Memory reallocation in CC index failed\n");
            error_val=OK_SUCCESS;
            break;
        case CC_CREATION_FAIL:
            fprintf(stderr, "Error: Error occured in Connected components creation\n");
            error_val=OK_SUCCESS;
            break;
        case LP_MALLOC_FAIL:
            fprintf(stderr, "Error: Memory allocation in list pool failed\n");
            error_val=OK_SUCCESS;
            break;
        case LP_REALLOC_FAIL:
            fprintf(stderr, "Error: Memory reallocation in list pool failed\n");
            error_val=OK_SUCCESS;
            break;

        case SCC_MALLOC_FAIL_BASIC_STRUCT:
            fprintf(stderr, "Error: malloc for SCC structure failed\n");
            error_val=OK_SUCCESS;
            break;
        case SCC_MALLOC_FAIL_IDS_ARRAY:
            fprintf(stderr, "Error: malloc for SCC structure's ids array failed\n");
            error_val=OK_SUCCESS;
            break;
        case SCC_MALLOC_FAIL_FLAGS_ARRAY:
            fprintf(stderr, "Error: malloc for SCC structure's bool array failed\n");
            error_val=OK_SUCCESS;
            break;
        case SCC_TARJAN_FAIL:
            fprintf(stderr, "Error: An error occurred while running Tarjan algorithm\n");
            error_val=OK_SUCCESS;
            break;
        case SCC_STACK_CREATION_FAIL:
            fprintf(stderr, "Error: An error occurred during stack creation for Tarjan\n");
            error_val=OK_SUCCESS;
            break;
        case TARJAN_STACK_INSERT_FAIL:
            fprintf(stderr, "Error: An error occurred during Tarjan execution: failed to add a node to stack\n");
            error_val=OK_SUCCESS;
            break;
        case TARJAN_BUFFER_POINTER_RETRIEVAL_FAIL:
            fprintf(stderr, "Error: An error occurred during Tarjan execution: failed to retrieve buffer pointer\n");
            error_val=OK_SUCCESS;
            break;
        case TARJAN_BUFFER_RETRIEVAL_FAIL:
            fprintf(stderr, "Error: An error occurred during Tarjan execution: failed to retrieve buffer\n");
            error_val=OK_SUCCESS;
            break;
        case TARJAN_LISTNODE_RETRIEVAL_FAIL:
            fprintf(stderr, "Error: An error occurred during Tarjan execution: failed to retrieve listnode\n");
            error_val=OK_SUCCESS;
            break;
        case TARJAN_STACK_PEEK_FAIL:
            fprintf(stderr, "Error: An error occurred during Tarjan execution: failed to peek top item in stack\n");
            error_val=OK_SUCCESS;
            break;
        case TARJAN_STACK_POP_FAIL:
            fprintf(stderr, "Error: An error occurred during Tarjan execution: failed to pop top item in stack\n");
            error_val=OK_SUCCESS;
            break;
        case TARJAN_COMPONENT_INIT_ARRAY_MALLOC_FAIL:
            fprintf(stderr, "Error: An error occurred during Tarjan execution: component's initial malloc for node array failed\n");
            error_val=OK_SUCCESS;
            break;
        case TARJAN_ADD_NODE_TO_COMPONENT_FAIL:
            fprintf(stderr, "Error: An error occurred during Tarjan execution: an error occurred while adding a new node to current component\n");
            error_val=OK_SUCCESS;
            break;
        case TARJAN_COMPONENT_FINALIZE_ARRAY_REALLOC_FAIL:
            fprintf(stderr, "Error: An error occurred during Tarjan execution: component's final realloc for node array failed\n");
            error_val=OK_SUCCESS;
            break;
        case SCC_ADD_COMPONENT_REALLOC_FAIL:
            fprintf(stderr, "Error: realloc for component's node array failed while adding a new node\n");
            error_val=OK_SUCCESS;
            break;
        case SCC_FINAL_REALLOC_FAIL:
            fprintf(stderr, "Error: final realloc for SCC's component array failed\n");
            error_val=OK_SUCCESS;
            break;
        case GRAPH_CREATE_STATIC_INDEX_SCCS_FAIL:
            fprintf(stderr, "Error: estimating Strongly Connected Components for static graph failed\n");
            error_val=OK_SUCCESS;
            break;
        case GRAIL_MALLOC_FAIL:
            fprintf(stderr, "Error: Allocating memory for grail index failed\n");
            error_val=OK_SUCCESS;
            break;
        case GRAPH_CREATE_STATIC_INDEX_ESTIMATE_SCCS_NEIGHBORS_FAIL:
            fprintf(stderr, "Error: estimating Strongly Connected Components' neighbors for static graph failed\n");
            error_val=OK_SUCCESS;
            break;
        case GRAPH_CREATE_STATIC_INDEX_GRAIL_FAIL:
            fprintf(stderr, "Error: creating grail index for static graph failed\n");
            error_val=OK_SUCCESS;
            break;
        case GRAPH_BFS_FAIL_GET_SCC_ID:
            fprintf(stderr, "Error (BFS): retrieving node's Strongly Connected Component ID failed\n");
            error_val=OK_SUCCESS;
            break;
        case GRAPH_BFS_FAIL_INSERT_TO_QUEUE:
            fprintf(stderr, "Error (BFS): inserting node to stack failed\n");
            error_val=OK_SUCCESS;
            break;
        case GRAPH_BFS_FAIL_INSERT_TO_HASHTABLE:
            fprintf(stderr, "Error (BFS): inserting node to hashtable failed\n");
            error_val=OK_SUCCESS;
            break;
        case GRAPH_BFS_FAIL_GET_QUEUE_SIZE:
            fprintf(stderr, "Error (BFS): retrieving stack size failed\n");
            error_val=OK_SUCCESS;
            break;
        case GRAPH_BFS_FAIL_QUEUE_POP:
            fprintf(stderr, "Error (BFS): stack pop() failed\n");
            error_val=OK_SUCCESS;
            break;
        case GRAPH_BFS_FAIL_QUEUE_PEEK:
            fprintf(stderr, "Error (BFS): stack peek() failed\n");
            error_val=OK_SUCCESS;
            break;
        case GRAPH_BFS_FAIL_GET_LIST_HEAD:
            fprintf(stderr, "Error (BFS): retrieving listnode pointer failed\n");
            error_val=OK_SUCCESS;
            break;
        case GRAPH_BFS_FAIL_GET_BUFFER:
            fprintf(stderr, "Error (BFS): retrieving buffer failed\n");
            error_val=OK_SUCCESS;
            break;
        case GRAPH_BFS_FAIL_GET_LISTNODE:
            fprintf(stderr, "Error (BFS): retrieving listnode failed\n");
            error_val=OK_SUCCESS;
            break;
        case GRAPH_BFS_FAIL_CHECK_VISITED:
            fprintf(stderr, "Error (BFS): checking if node is in hashtable failed\n");
            error_val=OK_SUCCESS;
            break;
        case GRAPH_BFS_FAIL_CHECK_BFS_TAG:
            fprintf(stderr, "Error (BFS): retrieving BFS tag from hastable failed\n");
            error_val=OK_SUCCESS;
            break;
        case JOBSCHEDULER_INIT_INVALID_NUM_OF_THREADS:
            fprintf(stderr, "Error creating Job Scheduler: non-positive number of threads provided\n");
            error_val=OK_SUCCESS;
            break;
        case JOBSCHEDULER_INIT_INITIAL_MALLOC_FAIL:
            fprintf(stderr, "Error creating Job Scheduler: malloc for basic struct failed\n");
            error_val=OK_SUCCESS;
            break;
        case JOBSCHEDULER_INIT_THREAD_POOL_MALLOC_FAIL:
            fprintf(stderr, "Error creating Job Scheduler: malloc for thread pool failed\n");
            error_val=OK_SUCCESS;
            break;
        case JOBSCHEDULER_INIT_PTHREAD_CREATE_FAIL:
            fprintf(stderr, "Error creating Job Scheduler: creating a thread failed\n");
            error_val=OK_SUCCESS;
            break;
        case JOBSCHEDULER_INIT_QUEUE_CREATION_FAIL:
            fprintf(stderr, "Error creating Job Scheduler: creating queue failed\n");
            error_val=OK_SUCCESS;
            break;
        case JOBSCHEDULER_SUBMIT_INSERT_TO_QUEUE_FAIL:
            fprintf(stderr, "Error (Job Scheduler): submitting new job (inserting to queue) failed\n");
            error_val=OK_SUCCESS;
            break;
    }
}

void print_errorv(int error_val){
    switch(error_val){
        case OK_SUCCESS:
            fprintf(stderr, "No error from structures.\n");
            error_val=OK_SUCCESS;
            break;
        case LIST_CR_MALLOC:
            fprintf(stderr, "Error: allocating memory in list creation\n");
            error_val=OK_SUCCESS;
            break;
        case LISTNODE_CR_MALLOC:
            fprintf(stderr, "Error: allocating memory in list node creation\n");
            error_val=OK_SUCCESS;
            break;
        case NULL_LIST:
            fprintf(stderr, "Error: list head called in function with NULL value\n");
            error_val=OK_SUCCESS;
            break;
        case NULL_NODE:
            fprintf(stderr, "Error: list node called in function with NULL value\n");
            error_val=OK_SUCCESS;
            break;
        case HASH_CR_MALLOC:
            fprintf(stderr, "Error: allocating memory in hash table creation\n");
            error_val=OK_SUCCESS;
            break;
        case HASH_BINS_CR_MALLOC:
            fprintf(stderr, "Error: allocating memory in hash table bins creation\n");
            error_val=OK_SUCCESS;
            break;
        case HASH_WRONG_PARMS:
            fprintf(stderr, "Error: wrong parameter in hash table creation:\nvalues must be ----size > 0----type = 1 || 0----\n");
            error_val=OK_SUCCESS;
            break;
        case NULL_HASH:
            fprintf(stderr, "Error: hash table called in function with NULL value\n");
            error_val=OK_SUCCESS;
            break;
        case NODE_MISSING:
            fprintf(stderr, "Error: request of a tag belonging to a non existing node.\n");
            break;
        case BUFFER_CR_MALLOC:
            fprintf(stderr, "Error: allocating struct Buffer\n");
            error_val=OK_SUCCESS;
            break;
        case BUFFER_CR_SPACE_MALLOC:
            fprintf(stderr, "Error: allocating memory space in struct Buffer\n");
            error_val=OK_SUCCESS;
            break;
        case BUFFER_DOUBLE_SIZE:
            fprintf(stderr, "Error: reallocating space and doubling size\n");
            error_val=OK_SUCCESS;
            break;
        case BUFFER_PTR_OUT_BOUNDS:
            fprintf(stderr, "Error: given ptr value exceeds the permitted values\n");
            error_val=OK_SUCCESS;
            break;
        case NULL_BUFFER:
            fprintf(stderr, "Error: Buffer pointer is NULL\n");
            error_val=OK_SUCCESS;
            break;
        case NULL_BUFFER_SPACE:
            fprintf(stderr, "Error: Buffer allocated space pointer points to NULL location\n");
            error_val=OK_SUCCESS;
            break;
        case INDEX_STRUCT_MALLOC_FAIL:
            fprintf(stderr, "Error: allocating space for index struct failed\n");
            error_val=OK_SUCCESS;
            break;
        case INDEX_STRUCT_MALLOC_INNER_FAIL:
            fprintf(stderr, "Error: allocating space in index\n");
            error_val=OK_SUCCESS;
            break;
        case INDEX_CR_WRONG_PARMS:
            fprintf(stderr, "Error: Wrong paramaters in index creation\n");
            error_val=OK_SUCCESS;
            break;
        case INDEX_NULL_HEAD:
            fprintf(stderr, "Error: Null pointer passed in index function \n");
            error_val=OK_SUCCESS;
            break;
        case INDEX_INSERT_NEGATIVE_NODEID :
            fprintf(stderr, "Error: negative id has been passed in index insert\n");
            error_val=OK_SUCCESS;
            break;
        case INDEX_REALLOC_FAIL:
            fprintf(stderr, "Error: reallocating memory in index failed\n");
            error_val=OK_SUCCESS;
            break;
        case INDEX_NODE_ID_OUT_BOUNDS:
            fprintf(stderr, "Error: node id doesn't exist in index (negative or has value than index size)\n");
            error_val=OK_SUCCESS;
            break;
        case INDEX_BUFFER_CR_FAIL:
            fprintf(stderr, "Error: index create function could not create buffer structure\n");
            error_val=OK_SUCCESS;
            break;
        case INDEX_ADD_EDGE_ALLOC_NEW_NODE:
            fprintf(stderr, "Error: index could not allocate memory for a new node\n");
            error_val=OK_SUCCESS;
            break;
        case INDEX_NEIGHBOR_EXISTS:
            fprintf(stderr, "Error: can't add edge to an existing neighbor\n");
            error_val=OK_SUCCESS;
            break;
        case INDEX_GET_LIST_NODE_FAIL:
            fprintf(stderr, "Error: index could not access list node\n");
            error_val=OK_SUCCESS;
            break;
        case INDEX_EDGE_CHECK_FAIL:
            fprintf(stderr, "Error: index could not check if edge exists\n");
            error_val=OK_SUCCESS;
            break;
        case GRAPH_NULL_POINTER_PROVIDED:
            fprintf(stderr, "Error: function called with graph parameter NULL\n");
            error_val=OK_SUCCESS;
            break;
        case GRAPH_CREATION_BASIC_STRUCT_MALLOC_FAIL:
            fprintf(stderr, "Error: malloc for graph structure failed\n");
            error_val=OK_SUCCESS;
            break;
        case GRAPH_CREATION_INDEX_MALLOC_FAIL:
            fprintf(stderr, "Error: malloc for a graph index failed\n");
            error_val=OK_SUCCESS;
            break;
        case GRAPH_CREATION_CR_LIST_FAIL:
            fprintf(stderr, "Error: creating an open_intlist for a graph index failed\n");
            error_val=OK_SUCCESS;
            break;
        case GRAPH_SEARCH_INIT_STRUCTS_FAIL:
            fprintf(stderr, "Error: initializing structs for graph search failed\n");
            error_val=OK_SUCCESS;
            break;
        case GRAPH_HASH_DESTROY_FAIL:
            fprintf(stderr, "Error: An error occurred while destroying graph structs\n");
            error_val=OK_SUCCESS;
            break;
        case VISITED_MALLOC_ERROR:
            fprintf(stderr, "Error: Memory allocation in visited structure failed\n");
            error_val=OK_SUCCESS;
            break;
        case VISITED_REALLOC_FAIL:
            fprintf(stderr, "Error: Memory reallocation in visited structure failed\n");
            error_val=OK_SUCCESS;
            break;
        case VISITED_WRONG_PARAMETERS:
            fprintf(stderr, "Error: Paramaters error in visited function\n");
            error_val=OK_SUCCESS;
            break;
        case CC_MALLOC_FAIL:
            fprintf(stderr, "Error: Memory allocation in CC index failed\n");
            error_val=OK_SUCCESS;
            break;
        case CC_REALLOC_FAIL:
            fprintf(stderr, "Error: Memory reallocation in CC index failed\n");
            error_val=OK_SUCCESS;
            break;
        case CC_CREATION_FAIL:
            fprintf(stderr, "Error: Error occured in Connected components creation\n");
            error_val=OK_SUCCESS;
            break;
        case LP_MALLOC_FAIL:
            fprintf(stderr, "Error: Memory allocation in list pool failed\n");
            error_val=OK_SUCCESS;
            break;
        case LP_REALLOC_FAIL:
            fprintf(stderr, "Error: Memory reallocation in list pool failed\n");
            error_val=OK_SUCCESS;
            break;

        case SCC_MALLOC_FAIL_BASIC_STRUCT:
            fprintf(stderr, "Error: malloc for SCC structure failed\n");
            error_val=OK_SUCCESS;
            break;
        case SCC_MALLOC_FAIL_IDS_ARRAY:
            fprintf(stderr, "Error: malloc for SCC structure's ids array failed\n");
            error_val=OK_SUCCESS;
            break;
        case SCC_MALLOC_FAIL_FLAGS_ARRAY:
            fprintf(stderr, "Error: malloc for SCC structure's bool array failed\n");
            error_val=OK_SUCCESS;
            break;
        case SCC_TARJAN_FAIL:
            fprintf(stderr, "Error: An error occurred while running Tarjan algorithm\n");
            error_val=OK_SUCCESS;
            break;
        case SCC_STACK_CREATION_FAIL:
            fprintf(stderr, "Error: An error occurred during stack creation for Tarjan\n");
            error_val=OK_SUCCESS;
            break;
        case TARJAN_STACK_INSERT_FAIL:
            fprintf(stderr, "Error: An error occurred during Tarjan execution: failed to add a node to stack\n");
            error_val=OK_SUCCESS;
            break;
        case TARJAN_BUFFER_POINTER_RETRIEVAL_FAIL:
            fprintf(stderr, "Error: An error occurred during Tarjan execution: failed to retrieve buffer pointer\n");
            error_val=OK_SUCCESS;
            break;
        case TARJAN_BUFFER_RETRIEVAL_FAIL:
            fprintf(stderr, "Error: An error occurred during Tarjan execution: failed to retrieve buffer\n");
            error_val=OK_SUCCESS;
            break;
        case TARJAN_LISTNODE_RETRIEVAL_FAIL:
            fprintf(stderr, "Error: An error occurred during Tarjan execution: failed to retrieve listnode\n");
            error_val=OK_SUCCESS;
            break;
        case TARJAN_STACK_PEEK_FAIL:
            fprintf(stderr, "Error: An error occurred during Tarjan execution: failed to peek top item in stack\n");
            error_val=OK_SUCCESS;
            break;
        case TARJAN_STACK_POP_FAIL:
            fprintf(stderr, "Error: An error occurred during Tarjan execution: failed to pop top item in stack\n");
            error_val=OK_SUCCESS;
            break;
        case TARJAN_COMPONENT_INIT_ARRAY_MALLOC_FAIL:
            fprintf(stderr, "Error: An error occurred during Tarjan execution: component's initial malloc for node array failed\n");
            error_val=OK_SUCCESS;
            break;
        case TARJAN_ADD_NODE_TO_COMPONENT_FAIL:
            fprintf(stderr, "Error: An error occurred during Tarjan execution: an error occurred while adding a new node to current component\n");
            error_val=OK_SUCCESS;
            break;
        case TARJAN_COMPONENT_FINALIZE_ARRAY_REALLOC_FAIL:
            fprintf(stderr, "Error: An error occurred during Tarjan execution: component's final realloc for node array failed\n");
            error_val=OK_SUCCESS;
            break;
        case SCC_ADD_COMPONENT_REALLOC_FAIL:
            fprintf(stderr, "Error: realloc for component's node array failed while adding a new node\n");
            error_val=OK_SUCCESS;
            break;
        case SCC_FINAL_REALLOC_FAIL:
            fprintf(stderr, "Error: final realloc for SCC's component array failed\n");
            error_val=OK_SUCCESS;
            break;
        case GRAPH_CREATE_STATIC_INDEX_SCCS_FAIL:
            fprintf(stderr, "Error: estimating Strongly Connected Components for static graph failed\n");
            error_val=OK_SUCCESS;
            break;
        case GRAIL_MALLOC_FAIL:
            fprintf(stderr, "Error: Allocating memory for grail index failed\n");
            error_val=OK_SUCCESS;
            break;
        case GRAPH_CREATE_STATIC_INDEX_ESTIMATE_SCCS_NEIGHBORS_FAIL:
            fprintf(stderr, "Error: estimating Strongly Connected Components' neighbors for static graph failed\n");
            error_val=OK_SUCCESS;
            break;
        case GRAPH_CREATE_STATIC_INDEX_GRAIL_FAIL:
            fprintf(stderr, "Error: creating grail index for static graph failed\n");
            error_val=OK_SUCCESS;
            break;
        case GRAPH_BFS_FAIL_GET_SCC_ID:
            fprintf(stderr, "Error (BFS): retrieving node's Strongly Connected Component ID failed\n");
            error_val=OK_SUCCESS;
            break;
        case GRAPH_BFS_FAIL_INSERT_TO_QUEUE:
            fprintf(stderr, "Error (BFS): inserting node to stack failed\n");
            error_val=OK_SUCCESS;
            break;
        case GRAPH_BFS_FAIL_INSERT_TO_HASHTABLE:
            fprintf(stderr, "Error (BFS): inserting node to hashtable failed\n");
            error_val=OK_SUCCESS;
            break;
        case GRAPH_BFS_FAIL_GET_QUEUE_SIZE:
            fprintf(stderr, "Error (BFS): retrieving stack size failed\n");
            error_val=OK_SUCCESS;
            break;
        case GRAPH_BFS_FAIL_QUEUE_POP:
            fprintf(stderr, "Error (BFS): stack pop() failed\n");
            error_val=OK_SUCCESS;
            break;
        case GRAPH_BFS_FAIL_QUEUE_PEEK:
            fprintf(stderr, "Error (BFS): stack peek() failed\n");
            error_val=OK_SUCCESS;
            break;
        case GRAPH_BFS_FAIL_GET_LIST_HEAD:
            fprintf(stderr, "Error (BFS): retrieving listnode pointer failed\n");
            error_val=OK_SUCCESS;
            break;
        case GRAPH_BFS_FAIL_GET_BUFFER:
            fprintf(stderr, "Error (BFS): retrieving buffer failed\n");
            error_val=OK_SUCCESS;
            break;
        case GRAPH_BFS_FAIL_GET_LISTNODE:
            fprintf(stderr, "Error (BFS): retrieving listnode failed\n");
            error_val=OK_SUCCESS;
            break;
        case GRAPH_BFS_FAIL_CHECK_VISITED:
            fprintf(stderr, "Error (BFS): checking if node is in hashtable failed\n");
            error_val=OK_SUCCESS;
            break;
        case GRAPH_BFS_FAIL_CHECK_BFS_TAG:
            fprintf(stderr, "Error (BFS): retrieving BFS tag from hastable failed\n");
            error_val=OK_SUCCESS;
            break;
        case JOBSCHEDULER_INIT_INVALID_NUM_OF_THREADS:
            fprintf(stderr, "Error creating Job Scheduler: non-positive number of threads provided\n");
            error_val=OK_SUCCESS;
            break;
        case JOBSCHEDULER_INIT_INITIAL_MALLOC_FAIL:
            fprintf(stderr, "Error creating Job Scheduler: malloc for basic struct failed\n");
            error_val=OK_SUCCESS;
            break;
        case JOBSCHEDULER_INIT_THREAD_POOL_MALLOC_FAIL:
            fprintf(stderr, "Error creating Job Scheduler: malloc for thread pool failed\n");
            error_val=OK_SUCCESS;
            break;
        case JOBSCHEDULER_INIT_PTHREAD_CREATE_FAIL:
            fprintf(stderr, "Error creating Job Scheduler: creating a thread failed\n");
            error_val=OK_SUCCESS;
            break;
        case JOBSCHEDULER_INIT_QUEUE_CREATION_FAIL:
            fprintf(stderr, "Error creating Job Scheduler: creating queue failed\n");
            error_val=OK_SUCCESS;
            break;
        case JOBSCHEDULER_SUBMIT_INSERT_TO_QUEUE_FAIL:
            fprintf(stderr, "Error (Job Scheduler): submitting new job (inserting to queue) failed\n");
            error_val=OK_SUCCESS;
            break;
    }
}
