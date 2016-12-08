#include "error.h"
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
        case GRAPH_SEARCH_INIT_STRUCTS_FAIL:
            fprintf(stderr, "Error: initializing structs for graph search failed\n");
            error_val=OK_SUCCESS;
            break;
        case GRAPH_HASH_DESTROY_FAIL:
            fprintf(stderr, "Error: An error occurred while destroying graph structs\n");
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
    }
}
