#ifndef _SCC_H_
#define _SCC_H_
#include "error.h"
#include "graph.h"

typedef struct SCC* pSCC;
typedef struct Component* pcomponent;

pSCC estimateStronglyConnectedComponents(pGraph);

int findNodeStronglyConnectedComponentID(pSCC components, uint32_t nodeId);

#endif  // _SCC_H_
