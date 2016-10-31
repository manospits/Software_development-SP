#include "error.h"
#include <stdio.h>

int error_val=OK_SUCCESS;

void print_error(){
    switch(error_val){
        case OK_SUCCESS:
            perror("No error from structures.\n");
            error_val=OK_SUCCESS;
            break;
        case LIST_CR_MALLOC:
            perror("Error: allocating memory in list creation\n");
            error_val=OK_SUCCESS;
            break;
        case LISTNODE_CR_MALLOC:
            perror("Error: allocating memory in list node creation\n");
            error_val=OK_SUCCESS;
            break;
        case NULL_LIST:
            perror("Error: list head called in function with NULL value\n");
            error_val=OK_SUCCESS;
            break;
        case NULL_NODE:
            perror("Error: list node called in function with NULL value\n");
            error_val=OK_SUCCESS;
            break;
        case HASH_CR_MALLOC:
            perror("Error: allocating memory in hash table creation\n");
            error_val=OK_SUCCESS;
            break;
        case HASH_BINS_CR_MALLOC:
            perror("Error: allocating memory in hash table bins creation\n");
            error_val=OK_SUCCESS;
            break;
        case HASH_WRONG_PARMS:
            fprintf(stderr, "Error: wrong parameter in hash table creation:\nvalues must be ----size > 0----type = 1 || 0----\n");
            error_val=OK_SUCCESS;
            break;
        case NULL_HASH:
            perror("Error: hash table called in function with NULL value\n");
            error_val=OK_SUCCESS;
            break;
        case BUFFER_CR_MALLOC:
            perror("Error: allocating struct Buffer\n");
            error_val=OK_SUCCESS;
            break;
        case BUFFER_CR_SPACE_MALLOC:
            perror("Error: allocating memory space in struct Buffer\n");
            error_val=OK_SUCCESS;
            break;
        case BUFFER_DOUBLE_SIZE:
            perror("Error: reallocating space and doubling size\n");
            error_val=OK_SUCCESS;
            break;
        case BUFFER_PTR_OUT_BOUNDS:
            perror("Error: given ptr value exceeds the permitted values\n");
            error_val=OK_SUCCESS;
            break;
        case NULL_BUFFER:
            perror("Error: Buffer pointer is NULL\n");
            error_val=OK_SUCCESS;
            break;
        case NULL_BUFFER_SPACE:
            perror("Error: Buffer allocated space pointer points to NULL location\n");
            error_val=OK_SUCCESS;
            break;
        case INDEX_STRUCT_MALLOC_FAIL:
            perror("Error: allocating space for index struct failed\n");
            error_val=OK_SUCCESS;
            break;
        case INDEX_INSERT_NEGATIVE_NODEID :
            perror("Error: negative id has been passed in index insert\n");
            error_val=OK_SUCCESS;
            break;
        case INDEX_REALLOC_FAIL:
            perror("Error: reallocating memory in index failed\n");
            error_val=OK_SUCCESS;
            break;
        case INDEX_NODE_ID_OUT_BOUNDS:
            perror("Error: node id doesn't exist in index (negative or has value than index size)\n");
            error_val=OK_SUCCESS;
            break;
        case INDEX_BUFFER_CR_FAIL:
            perror("Error: index create function could not create buffer structure\n");
            error_val=OK_SUCCESS;
            break;
        case INDEX_ADD_EDGE_ALLOC_NEW_NODE:
            perror("Error: index could not allocate memory for a new node\n");
            error_val=OK_SUCCESS;
            break;
        case INDEX_NEIGHBOR_EXISTS:
            perror("Error: can't add edge to an existing neighbor\n");
            error_val=OK_SUCCESS;
            break;
        case INDEX_GET_LIST_NODE_FAIL:
            perror("Error: index could not access list node\n");
            error_val=OK_SUCCESS;
            break;
        case INDEX_EDGE_CHECK_FAIL:
            perror("Error: index could not check if edge exists\n");
            error_val=OK_SUCCESS;
            break;
        case GRAPH_NULL_POINTER_PROVIDED:
            perror("Error: function called with graph parameter NULL\n");
            error_val=OK_SUCCESS;
            break;
        case GRAPH_CREATION_BASIC_STRUCT_MALLOC_FAIL:
            perror("Error: malloc for graph structure failed\n");
            error_val=OK_SUCCESS;
            break;
        case GRAPH_CREATION_INDEX_MALLOC_FAIL:
            perror("Error: malloc for a graph index failed\n");
            error_val=OK_SUCCESS;
            break;
        case GRAPH_SEARCH_INIT_STRUCTS_FAIL:
            perror("Error: initializing structs for graph search failed\n");
            error_val=OK_SUCCESS;
            break;
    }
}
