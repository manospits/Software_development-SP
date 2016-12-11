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
    uint32_t array_size;
    uint32_t *included_node_ids;
    uint32_t neighbors_count;
    uint32_t *neighbor_ids;
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


// adds node nodeId to last component
int add_node_to_component(pSCC sccs, uint32_t nodeId)
{
    if (sccs->components[sccs->components_count - 1].included_nodes_count == sccs->components[sccs->components_count - 1].array_size)
    {
        sccs->components[sccs->components_count - 1].array_size *= 2;
        if ((sccs->components[sccs->components_count - 1].included_node_ids = realloc(sccs->components[sccs->components_count - 1].included_node_ids, (sccs->components[sccs->components_count - 1].array_size)*sizeof(uint32_t))) == NULL)
        {
            error_val = SCC_ADD_COMPONENT_REALLOC_FAIL;
            return SCC_ADD_COMPONENT_REALLOC_FAIL;
        }
    }
    sccs->components[sccs->components_count - 1].included_node_ids[sccs->components[sccs->components_count - 1].included_nodes_count] = nodeId;
    sccs->id_belongs_to_component[nodeId] = sccs->components_count - 1;
    (sccs->components[sccs->components_count - 1].included_nodes_count)++;
    return 0;
}

rcode tarjan_rec(pGraph graph, pSCC sccs, phead stack, scc_flags *flags, uint32_t *tarjan_index_param, uint32_t nodeId)
{
    pBuffer temp_buffer;
    ptr buffer_ptr_to_listnode;
    plnode listnode;
    int i, temp;
    rcode return_value;
    flags[nodeId].index = *tarjan_index_param;
    flags[nodeId].lowlink = *tarjan_index_param;
    (*tarjan_index_param)++;
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
    if (buffer_ptr_to_listnode < -1)
    {   // an error occurred
        print_error();
        error_val = TARJAN_REC_BUFFER_POINTER_RETRIEVAL_FAIL;
        return TARJAN_REC_BUFFER_POINTER_RETRIEVAL_FAIL;
    }
    else if (buffer_ptr_to_listnode != -1)
    {   // if buffer_ptr_to_listnode == -1 then node has no neighbors, so continue to the end
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
                return_value = tarjan_rec(graph, sccs, stack, flags, tarjan_index_param, listnode->neighbor[i]);
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
    }
    // if nodeId is a root node, pop the stack and generate an SCC
    if (flags[nodeId].lowlink == flags[nodeId].index)
    {
        // initialize component
        sccs->components[sccs->components_count].component_id = sccs->components_count;
        sccs->components[sccs->components_count].included_nodes_count = 0;
        sccs->components[sccs->components_count].array_size = 8;
        if ((sccs->components[sccs->components_count].included_node_ids = malloc((sccs->components[sccs->components_count].array_size)*sizeof(uint32_t))) == NULL)
        {
            error_val = TARJAN_REC_COMPONENT_INIT_ARRAY_MALLOC_FAIL;
            return TARJAN_REC_COMPONENT_INIT_ARRAY_MALLOC_FAIL;
        }
        (sccs->components_count)++;
        do
        {
            if ((temp = peek_back(stack)) < 0)
            {
                print_error();
                error_val = TARJAN_REC_STACK_PEEK_FAIL;
                return TARJAN_REC_STACK_PEEK_FAIL;
            }
            if (pop_back(stack) < 0)
            {
                print_error();
                error_val = TARJAN_REC_STACK_POP_FAIL;
                return TARJAN_REC_STACK_POP_FAIL;
            }
            flags[temp].onStack = 0;
            if (add_node_to_component(sccs, temp) < 0)
            {
                print_error();
                error_val = TARJAN_REC_ADD_NODE_TO_COMPONENT_FAIL;
                return TARJAN_REC_ADD_NODE_TO_COMPONENT_FAIL;
            }
        }while(temp != nodeId);
        if ((sccs->components[sccs->components_count - 1].included_node_ids = realloc(sccs->components[sccs->components_count - 1].included_node_ids, (sccs->components[sccs->components_count - 1].included_nodes_count)*sizeof(uint32_t))) == NULL)
        {
            error_val = TARJAN_REC_COMPONENT_FINALIZE_ARRAY_REALLOC_FAIL;
            return TARJAN_REC_COMPONENT_FINALIZE_ARRAY_REALLOC_FAIL;
        }
        // array_size now is wrong; size now is included_nodes_count*sizeof()
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
    sccs->components_count = 0;
    sccs->number_of_nodes = ret_biggest_node(ret_outIndex(graph)) + 1;  // first node is 0
    if ((sccs->components = malloc((sccs->number_of_nodes)*sizeof(_component))) == NULL)
    {
        error_val = SCC_MALLOC_FAIL_IDS_ARRAY;
        free(sccs);
        return NULL;
    }
    if ((sccs->id_belongs_to_component = malloc((sccs->number_of_nodes)*sizeof(uint32_t))) == NULL)
    {
        error_val = SCC_MALLOC_FAIL_IDS_ARRAY;
        free(sccs->components);
        free(sccs);
        return NULL;
    }
    if ((flags = malloc((sccs->number_of_nodes)*sizeof(scc_flags))) == NULL)
    {
        error_val = SCC_MALLOC_FAIL_FLAGS_ARRAY;
        free(sccs->id_belongs_to_component);
        free(sccs->components);
        free(sccs);
        return NULL;
    }
    for (i = 0 ; i < sccs->number_of_nodes ; ++i)
    {
        flags[i].index = UINT_MAX;
        flags[i].onStack = 0;
        sccs->components[i].included_node_ids = NULL;
        sccs->components[i].neighbors_count = 0;
        sccs->components[i].neighbor_ids = NULL;
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
        // UINT_MAX == ~0 >> 2 <---- TODO: CHECK TO SEE IF IT SPEEDS UP
        if (flags[i].index == UINT_MAX) // UINT_MAX means that the node is undefined
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
    if ((sccs->components = realloc(sccs->components, sccs->components_count*sizeof(_component))) == NULL)
    {
        error_val = SCC_FINAL_REALLOC_FAIL;
        free(sccs->id_belongs_to_component);
        if (sccs->components != NULL) free(sccs->components);
        free(sccs);
        return NULL;
    }
    ds_list(stack);
    free(flags);
    //printf("Number of sccs created: %d nodes: %d\n", sccs->components_count, ret_biggest_node(ret_outIndex(graph)));  //DEBUG
    return sccs;
}

int estimateSCCsNeighbors(pSCC sccs, pGraph graph)
{
    if (sccs == NULL) return -1;
    pBuffer temp_buffer;
    ptr buffer_ptr_to_listnode;
    plnode listnode;
    uint32_t *component_flags, i, j, k;
    if ((component_flags = calloc(sccs->components_count, sizeof(uint32_t))) == NULL)
    {
        //error
    }
    // flags is used to check whether this neighbor has already been added to current component
    // it is initialized to zeros
    // condition is: if flags[neighbor] == component_id + 1
    // +1 is used because we start from 0, so we must catch this case somehow

    // find neighbors:
    for (j = 0 ; j < sccs->components_count ; ++j)
    {
        for (i = 0 ; i < sccs->components[j].included_nodes_count ; ++i)
        {
            buffer_ptr_to_listnode = getListHead(ret_outIndex(graph), sccs->components[j].included_node_ids[i]);
            if (buffer_ptr_to_listnode < -1)
            {   // an error occurred
                print_error();
                error_val = TARJAN_REC_BUFFER_POINTER_RETRIEVAL_FAIL;
                return TARJAN_REC_BUFFER_POINTER_RETRIEVAL_FAIL;
            }
            else if (buffer_ptr_to_listnode != -1)
            {   // if buffer_ptr_to_listnode == -1 then node has no neighbors, so continue to the end
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
                k = 0;
                while (listnode->neighbor[k] != -1)
                {
                    if (sccs->id_belongs_to_component[listnode->neighbor[k]] != sccs->components[j].component_id)
                    {   // if neighbor doesn't belong to the same component
                        if (component_flags[sccs->id_belongs_to_component[listnode->neighbor[k]]] != sccs->components[j].component_id + 1)
                        {   // if this neighbor hasn't been added yet
                            component_flags[sccs->id_belongs_to_component[listnode->neighbor[k]]] = sccs->components[j].component_id + 1;
                            //add_neighbor(&(sccs->components[j]), sccs->id_belongs_to_component[listnode->neighbor[k]]);
                        }
                    }
                    // get next neighbor
                    k++;
                    if (k == N)
                    {
                        if (listnode->nextListNode == -1)   // if there are no more neighbors, break
                            break;
                        if ((listnode = getListNode(temp_buffer, listnode->nextListNode)) == NULL)
                        {
                            print_error();
                            error_val = TARJAN_REC_LISTNODE_RETRIEVAL_FAIL;
                            return TARJAN_REC_LISTNODE_RETRIEVAL_FAIL;
                        }
                        k = 0;
                    }
                }
            }
        }
    }
    free(component_flags);
    return OK_SUCCESS;
}

int get_number_of_components(pSCC sccs)
{
    if (sccs == NULL) return -1;
    return sccs->components_count;
}

void get_component_neighbors(pComponent comp, uint32_t **neighbors, int *number_of_neighbors)
{
    if (comp == NULL || neighbors == NULL) return;
    *number_of_neighbors = comp->neighbors_count;
    *neighbors = comp->neighbor_ids;
}

int findNodeStronglyConnectedComponentID(pSCC sccomponents, uint32_t nodeId)
{
    if (sccomponents == NULL) return -1;
    return sccomponents->id_belongs_to_component[nodeId];
}

void iterateStronglyConnectedComponentID(pSCC components, pComponentCursor cursor)
{
    if (components == NULL || cursor == NULL)
        return; // TODO: error handling
    if (components->components_count == 0)
        cursor->component_ptr = NULL;
    else
        cursor->component_ptr = components->components;
}

char next_StronglyConnectedComponentID(pSCC components, pComponentCursor cursor)
{
    if (components == NULL) return 0;
    if (cursor->component_ptr < components->components)
    {
        fprintf(stderr, "out of bounds\n");
        return 0;
    }
    if (cursor->component_ptr - components->components + 1 < components->components_count)
    {
        (cursor->component_ptr)++;
        return 1;
    }
    return 0;
}

extern int bidirectional_bfs_inside_component(pSCC components, pGraph g, uint32_t from, uint32_t to, uint32_t component_id);

int estimateShortestPathStronglyConnectedComponents(pSCC components, pGraph graph, uint32_t source_node, uint32_t target_node)
{
    if (findNodeStronglyConnectedComponentID(components, source_node) != findNodeStronglyConnectedComponentID(components, target_node))
        return -1;
    return bidirectional_bfs_inside_component(components, graph, source_node, target_node, findNodeStronglyConnectedComponentID(components, source_node));
}

void destroyStronglyConnectedComponents(pSCC sccs)
{
    int i;
    // delete node array
    free(sccs->id_belongs_to_component);
    // delete components
    for (i = 0 ; i < sccs->components_count ; ++i)
        free(sccs->components[i].included_node_ids);
    free(sccs->components);
    // delete original structure
    free(sccs);
}
