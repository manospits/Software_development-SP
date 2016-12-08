#include "scc.h"
#include "intlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

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
    uint32_t number_of_nodes;
    uint32_t *id_belongs_to_component;
}_scc;

typedef struct scc_flags
{
    char onStack;
    uint32_t index;     // index == UINT_MAX indicates index is undefined
    uint32_t lowlink;
} scc_flags;


rcode tarjan(pSCC sccs, uint32_t nodeId, scc_flags *flags, uint32_t *index)
{
    flags[nodeId].index = *index;
    flags[nodeId].lowlink = *index;
    (*index)++;
    //push

    return OK_SUCCESS;
}

pSCC estimateStronglyConnectedComponents(pGraph graph)
{
    pSCC sccs;
    scc_flags *flags;
    phead stack;
    uint32_t index = 0;
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
    if ((flags = malloc((sccs->number_of_nodes)*sizeof(char))) == NULL)
    {
        error_val = SCC_MALLOC_FAIL_FLAGS_ARRAY;
        free(sccs->id_belongs_to_component);
        free(sccs);
        return NULL;
    }
    for (i = 0 ; i < sccs->number_of_nodes ; ++i)
    {
        flags[i].index = UINT_MAX;
        flags[i].onStack = 0;
    }
    //create stack
    if ((stack = cr_list()) == NULL)
    {
        print_error();
        free(flags);
        free(sccs->id_belongs_to_component);
        free(sccs);
        error_val = SCC_STACK_CREATION_FAIL;
        return NULL;
    }

    // run Tarjan algorithm for all nodes
    for (i = 0 ; i < sccs->number_of_nodes ; ++i)
        if (flags[i].index == UINT_MAX) // UINT_MAX == ~0 >> 2 <---- TODO: CHECK TO SEE IF IT SPEEDS UP
            if (tarjan(sccs, i, flags, &index) != OK_SUCCESS)
            {
                print_error();
                free(flags);
                free(sccs->id_belongs_to_component);
                free(sccs);
                error_val = SCC_TARJAN_FAIL;
                return NULL;
            }
    return sccs;
}

int findNodeStronglyConnectedComponentID(pSCC sccomponents, uint32_t nodeId)
{
    return sccomponents->id_belongs_to_component[nodeId];
}
