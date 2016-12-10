#include "scc.h"
#include "intlist.h"
#include "buffer.h"
#include "index.h"
#include "utils.h"
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

struct ComponentCursor{
    _component* component_ptr;  // pointer to current’s iteration component
    //
};




rcode tarjan_rec(pGraph graph, pSCC sccs, phead stack, scc_flags *flags, uint32_t *index, uint32_t nodeId)
{
    pBuffer temp_buffer;
    ptr buffer_ptr_to_listnode;
    plnode listnode;
    int i;
    rcode return_value;
    flags[nodeId].index = *index;
    flags[nodeId].lowlink = *index;
    (*index)++;
    if (insert_back(stack, nodeId) < 0)
    {
        print_error();
        error_val = TARJAN_REC_STACK_INSERT_FAIL;
        return TARJAN_REC_STACK_INSERT_FAIL;
    }
    flags[nodeId].onStack = 1;
    //
    // check node's neighbors
    buffer_ptr_to_listnode = getListHead(ret_outIndex(graph), nodeId);
    if (buffer_ptr_to_listnode == -1)
    {   // node has no neighbors
        //continue;
    }
    else if (buffer_ptr_to_listnode < 0)
    {   // an error occurred
        print_error();
        error_val = TARJAN_REC_BUFFER_POINTER_RETRIEVAL_FAIL;
        return TARJAN_REC_BUFFER_POINTER_RETRIEVAL_FAIL;
    }
    if ((temp_buffer = return_buffer(ret_outIndex(graph))) == NULL)
    {
        print_error();
        error_val = TARJAN_REC_BUFFER_RETRIEVAL_FAIL;
        return TARJAN_REC_BUFFER_RETRIEVAL_FAIL;
    }
    if ((listnode = getListNode(temp_buffer, buffer_ptr_to_listnode)) == NULL)
    {
        print_error();
        error_val = TARJAN_REC_INIT_LISTNODE_RETRIEVAL_FAIL;
        return TARJAN_REC_INIT_LISTNODE_RETRIEVAL_FAIL;
    }
    i = 0;
    while (listnode->neighbor[i] != -1)
    {
        if (flags[listnode->neighbor[i]].index == UINT_MAX)
        {
            return_value = tarjan_rec(graph, sccs, stack, flags, index, listnode->neighbor[i]);
            if (return_value != OK_SUCCESS)
                return return_value;
            flags[nodeId].lowlink = min(flags[nodeId].lowlink, flags[listnode->neighbor[i]].lowlink);
        }
        else if (flags[listnode->neighbor[i]].onStack == 1)
            flags[nodeId].lowlink = min(flags[nodeId].lowlink, flags[listnode->neighbor[i]].lowlink);
        // get next neighbor
        i++;
        if (i == N)
        {
            if (listnode->nextListNode == -1)   // if there are no more neighbors, break
                break;
            if ((listnode = getListNode(temp_buffer, listnode->nextListNode)) == NULL)
            {
                print_error();
                error_val = TARJAN_REC_LISTNODE_RETRIEVAL_FAIL;
                return TARJAN_REC_LISTNODE_RETRIEVAL_FAIL;
            }
            i = 0;
        }
    }
    // if nodeId is a root nonde, pop the stack and generate an SCC
    if (flags[nodeId].lowlink == flags[nodeId].index)
    {
        // create new component
        do
        {
            //peek
            //pop
        }while(1);
    }

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
            if (tarjan_rec(graph, sccs, stack, flags, &index, i) != OK_SUCCESS)
            {
                print_error();
                free(flags);
                free(sccs->id_belongs_to_component);
                free(sccs);
                ds_list(stack);
                error_val = SCC_TARJAN_FAIL;
                return NULL;
            }
    ds_list(stack);
    free(flags);
    return sccs;
}

int findNodeStronglyConnectedComponentID(pSCC sccomponents, uint32_t nodeId)
{
    return sccomponents->id_belongs_to_component[nodeId];
}

void iterateStronglyConnectedComponentID(pSCC components, pComponentCursor cursor)
{
    if (components->components_count == 0)
        cursor->component_ptr = NULL;
    else
        cursor->component_ptr = components->components;
}

char next_StronglyConnectedComponentID(pSCC components, pComponentCursor cursor)
{
    return 0;
}

int estimateShortestPathStronglyConnectedComponents(pSCC components, pGraph graph, uint32_t source_node, uint32_t target_node)
{
    return -1;
}

void destroyStronglyConnectedComponents(pSCC components)
{
    free(components->id_belongs_to_component);
    //delete/free components
    free(components);
}
