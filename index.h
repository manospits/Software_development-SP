#ifndef _INDEX_
#define _INDEX_

#include "buffer.h"
#include "error.h"
#include <stdint.h>

typedef struct NodeIndex* Index_ptr;

Index_ptr createNodeIndex();
    //Creates a struct NodeIndex and returns a pointer
    //to that structure in case of error NULL is returned
    //type:

rcode insertNode(const Index_ptr,uint32_t nodeId);
    //Inserts new node in index (it actually checks
    //if there is that position in array if not it makes the appropriate changes)

ptr getListHead(const Index_ptr,uint32_t nodeId);
    //returns ptr to lists head in case of error returns negative value

int edge_exists(const Index_ptr hindex,uint32_t nodeId,uint32_t neighbor);
    //1 exists-- 0 does not exist -- value < 0 error

rcode add_edge(const Index_ptr,uint32_t nodeId,uint32_t neighbor);
    //OK_SUCCESS if edge is added or negative value in case of error

rcode destroyNodeIndex(const Index_ptr);
    //returns OK_SUCCESS if there is no error negative value else

pBuffer return_buffer(const Index_ptr);
    //returns pointer to Buffer structure

int get_index_size(const Index_ptr);
    //returns index size

int get_node_number_of_edges(const Index_ptr hindex,uint32_t nodeId);
    //returns number of edges a node has


int get_number_of_edges(const Index_ptr hindex);
    //returns total number of edges
#endif
