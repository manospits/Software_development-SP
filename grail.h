#ifndef _GRAIL_
#define _GRAIL_
#include <stdint.h>
#include "scc.h"
#include "intlist.h"
#include "error.h"
#define LABEL_NUMBER 3

typedef char GRAIL_ANSWER;
typedef struct GrailIndex * Grail;

Grail buildGrailIndex(pSCC s,phead nodes,phead nodesp);
GRAIL_ANSWER isReachableGrailIndex(pSCC s,uint32_t source_node,uint32_t target_node);
rcode destroyGrailIndex(Grail g);

#endif

