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
    uint32_t parent;
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

rcode tarjan_iter(pGraph graph, pSCC sccs, phead stack, scc_flags *flags, uint32_t *tarjan_index_param, uint32_t nodeId)
{
    pBuffer temp_buffer;
    ptr buffer_ptr_to_listnode;
    plnode listnode;
    int i, temp_node;
    if (insert_back(stack, nodeId) < 0)
    {
        print_error();
        error_val = TARJAN_STACK_INSERT_FAIL;
        return TARJAN_STACK_INSERT_FAIL;
    }
    flags[nodeId].parent = nodeId;
    // initialize first component (at least one component will be created)
    sccs->components[sccs->components_count].component_id = sccs->components_count;
    sccs->components[sccs->components_count].included_nodes_count = 0;
    sccs->components[sccs->components_count].array_size = 8;
    if ((sccs->components[sccs->components_count].included_node_ids = malloc((sccs->components[sccs->components_count].array_size)*sizeof(uint32_t))) == NULL)
    {
        error_val = TARJAN_COMPONENT_INIT_ARRAY_MALLOC_FAIL;
        return TARJAN_COMPONENT_INIT_ARRAY_MALLOC_FAIL;
    }
    (sccs->components_count)++;
    // begin the iterative process
    temp_node = nodeId;
    while (get_size(stack) > 0)
    {
        if ((temp_node = peek_back(stack)) < 0)
        {
            print_error();
            //error_val
            return -1;
        }
        if (flags[temp_node].index == UINT_MAX)
        {   // if node gets visited for the 1st time
            flags[temp_node].index = *tarjan_index_param;
            flags[temp_node].lowlink = *tarjan_index_param;
            flags[temp_node].onStack = 1;
            (*tarjan_index_param)++;
        }
        // check node's neighbors
        buffer_ptr_to_listnode = getListHead(ret_outIndex(graph), nodeId);
        if (buffer_ptr_to_listnode < -1)
        {   // an error occurred
            print_error();
            error_val = TARJAN_BUFFER_POINTER_RETRIEVAL_FAIL;
            return TARJAN_BUFFER_POINTER_RETRIEVAL_FAIL;
        }
        // switch following 'if' with 'else if' to see if it improves performance
        if (buffer_ptr_to_listnode != -1)
        {   // if buffer_ptr_to_listnode == -1 then node has no neighbors, so continue to the end
            if ((temp_buffer = return_buffer(ret_outIndex(graph))) == NULL)
            {
                print_error();
                error_val = TARJAN_BUFFER_RETRIEVAL_FAIL;
                return TARJAN_BUFFER_RETRIEVAL_FAIL;
            }
            if ((listnode = getListNode(temp_buffer, buffer_ptr_to_listnode)) == NULL)
            {
                print_error();
                error_val = TARJAN_LISTNODE_RETRIEVAL_FAIL;
                return TARJAN_LISTNODE_RETRIEVAL_FAIL;
            }
            i = 0;
            while (listnode->neighbor[i] != -1)
            {   // for each neighbor do
                if (flags[listnode->neighbor[i]].index == UINT_MAX)
                {   // index is undefined -> neighbor not visited yet
                    if (insert_back(stack, listnode->neighbor[i]) < 0)
                    {
                        print_error();
                        error_val = TARJAN_STACK_INSERT_FAIL;
                        return TARJAN_STACK_INSERT_FAIL;
                    }
                    flags[listnode->neighbor[i]].parent = temp_node;
                    continue;
                }
                else if (flags[listnode->neighbor[i]].onStack)
                    flags[nodeId].lowlink = min(flags[nodeId].lowlink, flags[listnode->neighbor[i]].index);
                // get next neighbor
                i++;
                if (i == N)
                {
                    if (listnode->nextListNode == -1)   // if there are no more neighbors, break
                        break;
                    if ((listnode = getListNode(temp_buffer, listnode->nextListNode)) == NULL)
                    {
                        print_error();
                        error_val = TARJAN_LISTNODE_RETRIEVAL_FAIL;
                        return TARJAN_LISTNODE_RETRIEVAL_FAIL;
                    }
                    i = 0;
                }
            }
            //
            //flags[flags[temp_node].parent].lowlink = min(flags[flags[temp_node].parent].lowlink, flags[temp_node].lowlink);
            // add node to current component
            if (add_node_to_component(sccs, temp_node) < 0)
            {
                print_error();
                error_val = TARJAN_ADD_NODE_TO_COMPONENT_FAIL;
                return TARJAN_ADD_NODE_TO_COMPONENT_FAIL;
            }
            // check if we have finished adding to the component all the nodes that belong there
            if (flags[temp_node].lowlink == flags[temp_node].index && flags[temp_node].parent != temp_node)
            {   // if lowlink == index, then current component is finished and a new component must be created for the next nodes to enter.
                // However, if the bottom of the stack has been reached (temp_node == parent), then there are no more nodes, so a new component must not be created.
                //finalize previous component
                if ((sccs->components[sccs->components_count - 1].included_node_ids = realloc(sccs->components[sccs->components_count - 1].included_node_ids, (sccs->components[sccs->components_count - 1].included_nodes_count)*sizeof(uint32_t))) == NULL)
                {
                    error_val = TARJAN_COMPONENT_FINALIZE_ARRAY_REALLOC_FAIL;
                    return TARJAN_COMPONENT_FINALIZE_ARRAY_REALLOC_FAIL;
                }
                // ATTENTION: array_size field now is wrong; size now is included_nodes_count*sizeof()
                // initialize new component
                sccs->components[sccs->components_count].component_id = sccs->components_count;
                sccs->components[sccs->components_count].included_nodes_count = 0;
                sccs->components[sccs->components_count].array_size = 8;
                if ((sccs->components[sccs->components_count].included_node_ids = malloc((sccs->components[sccs->components_count].array_size)*sizeof(uint32_t))) == NULL)
                {
                    error_val = TARJAN_COMPONENT_INIT_ARRAY_MALLOC_FAIL;
                    return TARJAN_COMPONENT_INIT_ARRAY_MALLOC_FAIL;
                }
                (sccs->components_count)++;
            }
            if (pop_back(stack) < 0)
            {
                print_error();
                error_val = TARJAN_STACK_POP_FAIL;
                return TARJAN_STACK_POP_FAIL;
            }
        }
    }

    return OK_SUCCESS;
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
        error_val = TARJAN_STACK_INSERT_FAIL;
        return TARJAN_STACK_INSERT_FAIL;
    }
    flags[nodeId].onStack = 1;
    //
    // check node's neighbors
    buffer_ptr_to_listnode = getListHead(ret_outIndex(graph), nodeId);
    if (buffer_ptr_to_listnode < -1)
    {   // an error occurred
        print_error();
        error_val = TARJAN_BUFFER_POINTER_RETRIEVAL_FAIL;
        return TARJAN_BUFFER_POINTER_RETRIEVAL_FAIL;
    }
    else if (buffer_ptr_to_listnode != -1)
    {   // if buffer_ptr_to_listnode == -1 then node has no neighbors, so continue to the end
        if ((temp_buffer = return_buffer(ret_outIndex(graph))) == NULL)
        {
            print_error();
            error_val = TARJAN_BUFFER_RETRIEVAL_FAIL;
            return TARJAN_BUFFER_RETRIEVAL_FAIL;
        }
        if ((listnode = getListNode(temp_buffer, buffer_ptr_to_listnode)) == NULL)
        {
            print_error();
            error_val = TARJAN_LISTNODE_RETRIEVAL_FAIL;
            return TARJAN_LISTNODE_RETRIEVAL_FAIL;
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
            else if (flags[listnode->neighbor[i]].onStack)
                flags[nodeId].lowlink = min(flags[nodeId].lowlink, flags[listnode->neighbor[i]].index);
            // get next neighbor
            i++;
            if (i == N)
            {
                if (listnode->nextListNode == -1)   // if there are no more neighbors, break
                    break;
                if ((listnode = getListNode(temp_buffer, listnode->nextListNode)) == NULL)
                {
                    print_error();
                    error_val = TARJAN_LISTNODE_RETRIEVAL_FAIL;
                    return TARJAN_LISTNODE_RETRIEVAL_FAIL;
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
            error_val = TARJAN_COMPONENT_INIT_ARRAY_MALLOC_FAIL;
            return TARJAN_COMPONENT_INIT_ARRAY_MALLOC_FAIL;
        }
        (sccs->components_count)++;
        do
        {
            if ((temp = peek_back(stack)) < 0)
            {
                print_error();
                error_val = TARJAN_STACK_PEEK_FAIL;
                return TARJAN_STACK_PEEK_FAIL;
            }
            if (pop_back(stack) < 0)
            {
                print_error();
                error_val = TARJAN_STACK_POP_FAIL;
                return TARJAN_STACK_POP_FAIL;
            }
            flags[temp].onStack = 0;
            if (add_node_to_component(sccs, temp) < 0)
            {
                print_error();
                error_val = TARJAN_ADD_NODE_TO_COMPONENT_FAIL;
                return TARJAN_ADD_NODE_TO_COMPONENT_FAIL;
            }
        }while(temp != nodeId);
        if ((sccs->components[sccs->components_count - 1].included_node_ids = realloc(sccs->components[sccs->components_count - 1].included_node_ids, (sccs->components[sccs->components_count - 1].included_nodes_count)*sizeof(uint32_t))) == NULL)
        {
            error_val = TARJAN_COMPONENT_FINALIZE_ARRAY_REALLOC_FAIL;
            return TARJAN_COMPONENT_FINALIZE_ARRAY_REALLOC_FAIL;
        }
        // ATTENTION: array_size field now is wrong; size now is included_nodes_count*sizeof()
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
    //initialize structures
    for (i = 0 ; i < sccs->number_of_nodes ; ++i)
    {
        flags[i].onStack = 0;
        flags[i].index = UINT_MAX;
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
        if (flags[i].index == UINT_MAX) // UINT_MAX means that the node is undefined - could be replaced with special field, but this way uses less memory
            if (tarjan_iter(graph, sccs, stack, flags, &index, i) != OK_SUCCESS)
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

int add_component_neighbor(pComponent comp, uint32_t neighborId)
{
    if (comp->neighbors_count == 0)
    {
        if ((comp->neighbor_ids = malloc(sizeof(uint32_t))) == NULL)
        {
            error_val = SCC_ADD_COMPONENT_NEIGHBOR_MALLOC_FAIL;
            return SCC_ADD_COMPONENT_NEIGHBOR_MALLOC_FAIL;
        }
        comp->neighbor_ids[0] = neighborId;
        (comp->neighbors_count)++;
    }
    else
    {
        (comp->neighbors_count)++;
        if ((comp->neighbor_ids = realloc(comp->neighbor_ids, (comp->neighbors_count)*sizeof(uint32_t))) == NULL)
        {
            error_val = SCC_ADD_COMPONENT_NEIGHBOR_REALLOC_FAIL;
            return SCC_ADD_COMPONENT_NEIGHBOR_REALLOC_FAIL;
        }
        comp->neighbor_ids[comp->neighbors_count - 1] = neighborId;
    }
    return OK_SUCCESS;
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
        error_val = SCC_NEIGHBORS_FLAGS_CALLOC_FAIL;
        return SCC_NEIGHBORS_FLAGS_CALLOC_FAIL;
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
                error_val = SCC_NEIGHBORS_BUFFER_POINTER_RETRIEVAL_FAIL;
                return SCC_NEIGHBORS_BUFFER_POINTER_RETRIEVAL_FAIL;
            }
            else if (buffer_ptr_to_listnode != -1)
            {   // if buffer_ptr_to_listnode == -1 then node has no neighbors, so continue to the end
                if ((temp_buffer = return_buffer(ret_outIndex(graph))) == NULL)
                {
                    print_error();
                    error_val = SCC_NEIGHBORS_BUFFER_RETRIEVAL_FAIL;
                    return SCC_NEIGHBORS_BUFFER_RETRIEVAL_FAIL;
                }
                if ((listnode = getListNode(temp_buffer, buffer_ptr_to_listnode)) == NULL)
                {
                    print_error();
                    error_val = SCC_NEIGHBORS_LISTNODE_RETRIEVAL_FAIL;
                    return SCC_NEIGHBORS_LISTNODE_RETRIEVAL_FAIL;
                }
                k = 0;
                while (listnode->neighbor[k] != -1)
                {
                    if (sccs->id_belongs_to_component[listnode->neighbor[k]] != sccs->components[j].component_id)
                    {   // if neighbor doesn't belong to the same component
                        if (component_flags[sccs->id_belongs_to_component[listnode->neighbor[k]]] != sccs->components[j].component_id + 1)
                        {   // if this neighbor hasn't been added yet
                            component_flags[sccs->id_belongs_to_component[listnode->neighbor[k]]] = sccs->components[j].component_id + 1;
                            if (add_component_neighbor(&(sccs->components[j]), sccs->id_belongs_to_component[listnode->neighbor[k]]) < 0)
                            {
                                error_val = SCC_NEIGHBORS_ADD_COMPONENT_FAIL;
                                return SCC_NEIGHBORS_ADD_COMPONENT_FAIL;
                            }
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
                            error_val = SCC_NEIGHBORS_LISTNODE_RETRIEVAL_FAIL;
                            return SCC_NEIGHBORS_LISTNODE_RETRIEVAL_FAIL;
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
/*
pComponent get_SCComponents(pSCC sccs)
{
    if (sccs == NULL) return NULL;
    return sccs->components;
}
*/
void get_component_neighbors(pSCC sccs, uint32_t componentId, uint32_t **neighbors, uint32_t *number_of_neighbors)
{
    if (sccs == NULL || componentId > sccs->components_count) return;   //error
    *number_of_neighbors = sccs->components[componentId].neighbors_count;
    if (neighbors != NULL) *neighbors = sccs->components[componentId].neighbor_ids;
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
    /*if (findNodeStronglyConnectedComponentID(components, source_node) != findNodeStronglyConnectedComponentID(components, target_node))
        return -1;*/
    return bidirectional_bfs_inside_component(components, graph, source_node, target_node, findNodeStronglyConnectedComponentID(components, source_node));
}

void destroyStronglyConnectedComponents(pSCC sccs)
{
    int i;
    // delete node array
    free(sccs->id_belongs_to_component);
    // delete components
    for (i = 0 ; i < sccs->components_count ; ++i)
    {
        free(sccs->components[i].included_node_ids);
        free(sccs->components[i].neighbor_ids);
    }
    free(sccs->components);
    // delete original structure
    free(sccs);
}
