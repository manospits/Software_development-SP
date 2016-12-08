#ifndef _SCC_H_
#define _SCC_H_
#include "error.h"
#include "graph.h"

typedef struct SCC* pSCC;
typedef struct Component* pcomponent;

pSCC estimateStronglyConnectedComponents(pGraph);

#endif  // _SCC_H_
