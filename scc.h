#ifndef _SCC_H_
#define _SCC_H_
#include "error.h"
#include "graph.h"

typedef struct SCC* pSCC;
typedef struct Component* pComponent;
typedef struct ComponentCursor* pComponentCursor;

pSCC estimateStronglyConnectedComponents(pGraph);

int findNodeStronglyConnectedComponentID(pSCC components, uint32_t nodeId);

void iterateStronglyConnectedComponentID(pSCC components, pComponentCursor cursor);

char next_StronglyConnectedComponentID(pSCC components, pComponentCursor cursor);

int estimateShortestPathStronglyConnectedComponents(pSCC components, pGraph graph, uint32_t source_node, uint32_t target_node);

int ret_number_of_components(pSCC);

void destroyStronglyConnectedComponents(pSCC components);

#endif  // _SCC_H_
