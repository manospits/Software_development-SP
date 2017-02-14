#ifndef _GRAIL_
#define _GRAIL_
#include <stdint.h>
#include "scc.h"
#include "intlist.h"
#include "error.h"
#define LABEL_NUMBER 5

typedef char GRAIL_ANSWER;
typedef struct GrailIndex * Grail;

Grail buildGrailIndex(pSCC s,phead nodes,phead nodesp); //create grail index

GRAIL_ANSWER isReachableGrailIndex(Grail index,pSCC s, uint32_t source_node,uint32_t target_node); //use grail index to answer if two nodes are connected or not

rcode destroyGrailIndex(Grail g); //destroy grail index

#endif

