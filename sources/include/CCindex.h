#ifndef _CCINDEX_
#define _CCINDEX_
#define UINDEX_INIT_SIZE 2
#define CCINDEX_INIT_SIZE 2
#define UPDATED_INIT_SIZE 2
#define METRIC_VAL 0.005
#include "error.h"
#include "graph.h"
#include "intlist.h"
#include "visited.h"
#include <stdint.h>

typedef struct CC *CC_index;

CC_index CC_create_index(pGraph g);                                 //create CC index structure

rcode CC_insertNewEdge(CC_index c,uint32_t nodeida,uint32_t nodeidb);                    //add adge in cc index
rcode CC_insertNewEdge_t(CC_index c,uint32_t nodeida,uint32_t nodeidb,uint32_t version); //thread version of the above function

rcode CC_rebuildIndexes(CC_index c);                                //rebuild CC index structure

rcode CC_destroy(CC_index c);                                       //destroy cc index structure

rcode check_rebuild(CC_index c);                                    //check if there is an need for a rebuild and perform it
rcode check_rebuild_t(CC_index c,int *queries,int *update_queries); //thread version of the above function

int CC_same_component_2(CC_index c,uint32_t nodeida ,uint32_t nodeidb); //checks if two nodes belong in the same connected component
int CC_same_component_2_t(CC_index c,uint32_t nodeida ,uint32_t nodeidb,
                          uint32_t version,int *queries,
                          int *update_queries);                         //thread version of the above function
void print_max(CC_index);                                           //prints biggest CC_index
#endif
