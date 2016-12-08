#ifndef _CCINDEX_
#define _CCINDEX_
#define UINDEX_INIT_SIZE 2
#define CCINDEX_INIT_SIZE 2
#define UPDATED_INIT_SIZE 2
#define METRIC_INIT_VALUE 100
#include "error.h"
#include <stdint.h>

typedef struct CC *CC_index;

CC_index CC_create_index();
rcode CC_insertNewEdge(CC_index c,uint32_t nodeida,uint32_t nodeidb);
int CC_findNodeConnectedComponentID(CC_index c,uint32_t nodeid);
rcode CC_rebuildIndexes(CC_index c);
rcode CC_destroy(CC_index c);
int CC_same_component(CC_index c, uint32_t a, uint32_t b);

#endif
