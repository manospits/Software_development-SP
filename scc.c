#include "scc.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct Component
{
    uint32_t component_id;
    uint32_t included_nodes_count;
    uint32_t *included_node_ids;
}_component;

typedef struct SCC
{
    _component *components;
    uint32_t components_count;
    uint32_t *id_belongs_to_component;
    char *id_added;
    uint32_t number_of_nodes;
}_scc;


pSCC estimateStronglyConnectedComponents(pGraph graph)
{
    pSCC sccs;
    int i;
    // create & initialize scss
    if ((sccs = malloc(sizeof(_scc))) == NULL)
    {
        error_val = SCC_MALLOC_FAIL_BASIC_STRUCT;
        return NULL;
    }
    sccs->components = NULL;
    sccs->components_count = 0;
    sccs->number_of_nodes = ret_biggest_node(ret_outIndex(graph));
    if ((sccs->id_belongs_to_component = malloc((sccs->number_of_nodes)*sizeof(uint32_t))) == NULL)
    {
        error_val = SCC_MALLOC_FAIL_IDS_ARRAY;
        free(sccs);
        return NULL;
    }
    if ((sccs->id_added = malloc((sccs->number_of_nodes)*sizeof(char))) == NULL)
    {
        error_val = SCC_MALLOC_FAIL_BOOL_ARRAY;
        free(sccs->id_belongs_to_component);
        free(sccs);
        return NULL;
    }
    for (i = 0 ; i < sccs->number_of_nodes ; ++i)
        sccs->id_added[i] = 0;
    // run Tarjan algorithm for all nodes
    for (i = 0 ; i < sccs->number_of_nodes ; ++i)
        if (!sccs->id_added[i])
            ;//tarjan
    return sccs;
}
