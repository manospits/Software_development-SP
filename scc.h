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

int estimateSCCsNeighbors(pSCC sccs, pGraph graph);

int get_number_of_components(pSCC);

void get_component_neighbors(pSCC, uint32_t componentId, uint32_t **neighbors, int *number_of_neighbors);

void destroyStronglyConnectedComponents(pSCC components);

#endif  // _SCC_H_
