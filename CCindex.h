#ifndef _CCINDEX_
#define _CCINDEX_
#define UINDEX_INIT_SIZE 2
#define CCINDEX_INIT_SIZE 2
#define UPDATED_INIT_SIZE 2
#define METRIC_VAL 0.005
#include "error.h"
#include "graph.h"
#include "intlist.h"
#include <stdint.h>

typedef struct CC *CC_index;

CC_index CC_create_index(pGraph g);
rcode CC_insertNewEdge(CC_index c,uint32_t nodeida,uint32_t nodeidb);
int CC_findNodeConnectedComponentID(CC_index c,uint32_t nodeid);
rcode CC_rebuildIndexes(CC_index c);
rcode CC_destroy(CC_index c);
rcode check_rebuild(CC_index c);
int CC_same_component(CC_index c, uint32_t a, uint32_t b);
int CC_same_component_2(CC_index c,uint32_t nodeida ,uint32_t nodeidb);
int CC_same_component_2_t(CC_index c,uint32_t nodeida ,uint32_t nodeidb,uint32_t version,phead idlist);
void print_max(CC_index);
#endif
