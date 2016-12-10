#ifndef _ERROR_
#define _ERROR_

typedef int rcode;
void print_error();

extern int error_val;

//ERROR CODES EXPLANATION
#define OK_SUCCESS 0
#define LIST_CR_MALLOC -1
#define LISTNODE_CR_MALLOC -2
#define NULL_LIST -3
#define NULL_NODE -4
#define EMPTY_LIST -5
#define HASH_CR_MALLOC -6
#define HASH_BINS_CR_MALLOC -7
#define HASH_WRONG_PARMS -8
#define NULL_HASH -9
#define BUFFER_CR_MALLOC -10
#define BUFFER_CR_SPACE_MALLOC -11
#define BUFFER_DOUBLE_SIZE -12
#define BUFFER_PTR_OUT_BOUNDS -13
#define NULL_BUFFER -14
#define NULL_BUFFER_SPACE -15
#define INDEX_STRUCT_MALLOC_FAIL -16
#define INDEX_STRUCT_MALLOC_INNER_FAIL -17
#define INDEX_CR_WRONG_PARMS -18
#define INDEX_NULL_HEAD -19
#define INDEX_INSERT_NEGATIVE_NODEID -20
#define INDEX_REALLOC_FAIL -21
#define INDEX_NODE_ID_OUT_BOUNDS -22
#define INDEX_BUFFER_CR_FAIL -23
#define INDEX_ADD_EDGE_ALLOC_NEW_NODE -24
#define INDEX_GET_LIST_NODE_FAIL -25
#define INDEX_NEIGHBOR_EXISTS -26
#define INDEX_EDGE_CHECK_FAIL -27
#define GRAPH_NULL_POINTER_PROVIDED -28
#define GRAPH_CREATION_BASIC_STRUCT_MALLOC_FAIL -29
#define GRAPH_CREATION_INDEX_MALLOC_FAIL -30
#define GRAPH_SEARCH_INIT_STRUCTS_FAIL -31
#define GRAPH_SEARCH_PATH_NOT_FOUND -32
#define NODE_MISSING -33
#define GRAPH_HASH_DESTROY_FAIL -34
#define CC_MALLOC_FAIL -35
#define CC_REALLOC_FAIL -36
#define LP_MALLOC_FAIL -37
#define LP_REALLOC_FAIL -38
#define SCC_MALLOC_FAIL_BASIC_STRUCT -39
#define SCC_MALLOC_FAIL_IDS_ARRAY -40
#define SCC_MALLOC_FAIL_FLAGS_ARRAY -41
#define SCC_TARJAN_FAIL -42
#define SCC_STACK_CREATION_FAIL -43
#define TARJAN_REC_STACK_INSERT_FAIL -44
#define TARJAN_REC_BUFFER_POINTER_RETRIEVAL_FAIL -44
#define TARJAN_REC_BUFFER_RETRIEVAL_FAIL -45
#define TARJAN_REC_INIT_LISTNODE_RETRIEVAL_FAIL -46
#define TARJAN_REC_LISTNODE_RETRIEVAL_FAIL -46

#endif
