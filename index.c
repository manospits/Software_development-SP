#include "error.h"
#include "index.h"
#include "buffer.h"
#include <stdlib.h>
#include <stdio.h>
#ifndef INDEX_INIT_SIZE
#define INDEX_INIT_SIZE 2
#endif

typedef struct Inode{
    int edges;
    ptr List_start;
    ptr list_node_for_next_neigbor;
    int node_index_for_next_neighbor;
}Inode;

typedef struct NodeIndex{
    Inode *index;
    pBuffer buffer;
    int size;
    int edges;
    int nodes;
}NodeIndex;


Index_ptr createNodeIndex(){
    Index_ptr tmp;
    int i;
    if((tmp=malloc(sizeof(NodeIndex)))==NULL){
        error_val=INDEX_STRUCT_MALLOC_FAIL;
        return NULL;
    }
    if((tmp->index=malloc(sizeof(Inode)*INDEX_INIT_SIZE))==NULL){
        error_val=INDEX_STRUCT_MALLOC_INNER_FAIL;
        free(tmp);
        return NULL;
    }
    if((tmp->buffer=createBuffer())==NULL){
        free(tmp);
        print_error();
        error_val=INDEX_BUFFER_CR_FAIL;
        return NULL;
    }
    for(i=0;i<INDEX_INIT_SIZE;i++){
        tmp->index[i].List_start=-1;
        tmp->index[i].list_node_for_next_neigbor=-1;
        tmp->index[i].node_index_for_next_neighbor=-1;
        tmp->index[i].edges=0;
    }
    tmp->edges=0;
    tmp->nodes=0;
    tmp->size=INDEX_INIT_SIZE;
    error_val=OK_SUCCESS;
    return tmp;
}

rcode insertNode(const Index_ptr hindex,uint32_t nodeId){
    if(hindex==NULL){
        error_val=INDEX_NULL_HEAD;
        return INDEX_NULL_HEAD;
    }
    if(nodeId<0){
        error_val=INDEX_INSERT_NEGATIVE_NODEID;
        return INDEX_INSERT_NEGATIVE_NODEID;
    }
    if(nodeId<hindex->size){
        error_val=OK_SUCCESS;
        if(nodeId>hindex->nodes){
            hindex->nodes=nodeId;
        }
        return OK_SUCCESS;
    }
    else{
        int prev_size=hindex->size;
        int next_size=hindex->size,i;
        while(next_size<=nodeId){
            next_size<<=1;
        }
        if((hindex->index=realloc(hindex->index,next_size*sizeof(Inode)))==NULL){
            error_val=INDEX_REALLOC_FAIL;
            return INDEX_REALLOC_FAIL;
        }
        for(i=prev_size;i<next_size;i++){
            hindex->index[i].List_start=-1;
            hindex->index[i].list_node_for_next_neigbor=-1;
            hindex->index[i].node_index_for_next_neighbor=-1;
            hindex->index[i].edges=0;
        }
        hindex->size=next_size;
        if(nodeId>hindex->nodes){
            hindex->nodes=nodeId;
        }
    }
    error_val=OK_SUCCESS;
    return OK_SUCCESS;
}

ptr getListHead(const Index_ptr hindex,uint32_t nodeId){
    if(hindex==NULL){
        error_val=INDEX_NULL_HEAD;
        return INDEX_NULL_HEAD;
    }
    if(nodeId<0){
        error_val=INDEX_INSERT_NEGATIVE_NODEID;
        return INDEX_INSERT_NEGATIVE_NODEID;
    }
    if(nodeId>hindex->size){
        error_val=INDEX_NODE_ID_OUT_BOUNDS;
        return INDEX_NODE_ID_OUT_BOUNDS;
    }
    error_val=OK_SUCCESS;
    return hindex->index[nodeId].List_start;
}

int get_node_number_of_edges(const Index_ptr hindex,uint32_t nodeId){
    if(hindex==NULL){
        error_val=INDEX_NULL_HEAD;
        return INDEX_NULL_HEAD;
    }
    if(nodeId<0){
        error_val=INDEX_INSERT_NEGATIVE_NODEID;
        return INDEX_INSERT_NEGATIVE_NODEID;
    }
    if(nodeId>hindex->size){
        error_val=INDEX_NODE_ID_OUT_BOUNDS;
        return INDEX_NODE_ID_OUT_BOUNDS;
    }
    error_val=OK_SUCCESS;
    return hindex->index[nodeId].edges;

}

int* get_node_number_of_edges_2(const Index_ptr hindex,uint32_t nodeId){
    if(hindex==NULL){
        error_val=INDEX_NULL_HEAD;
        return NULL;
    }
    if(nodeId<0){
        error_val=INDEX_INSERT_NEGATIVE_NODEID;
        return NULL;
    }
    if(nodeId>hindex->size){
        error_val=INDEX_NODE_ID_OUT_BOUNDS;
        return NULL;
    }
    error_val=OK_SUCCESS;
    return &hindex->index[nodeId].edges;

}
int edge_exists(const Index_ptr hindex,uint32_t nodeId,uint32_t neighbor){
    plnode tmplnode;
    int i;
    if(hindex==NULL){
        error_val=INDEX_NULL_HEAD;
        return INDEX_NULL_HEAD;
    }
    if(nodeId<0||neighbor<0){
        error_val=INDEX_INSERT_NEGATIVE_NODEID;
        return INDEX_INSERT_NEGATIVE_NODEID;
    }
    if(nodeId>hindex->size){
        error_val=INDEX_NODE_ID_OUT_BOUNDS;
        return INDEX_NODE_ID_OUT_BOUNDS;
    }
    if(hindex->index[nodeId].List_start==-1){
        return 0;
    }
    ptr nextlptr=hindex->index[nodeId].List_start;
    do{
        tmplnode=getListNode(hindex->buffer,nextlptr);
        if(tmplnode==NULL){
            print_error();
            error_val=INDEX_GET_LIST_NODE_FAIL;
            return INDEX_GET_LIST_NODE_FAIL;
        }
        for(i=0;i<N;i++){
            if(tmplnode->neighbor[i]==neighbor){
                error_val=OK_SUCCESS;
                return 1;
            }
            else if (tmplnode->neighbor[i]==-1) {
                break;
            }
        }
        nextlptr=tmplnode->nextListNode;
    }while(nextlptr!=-1);
    error_val=OK_SUCCESS;
    return 0;
}

rcode add_edge(const Index_ptr hindex,uint32_t nodeId,uint32_t neighbor){
    plnode tmplnode;
    if(hindex==NULL){
        error_val=INDEX_NULL_HEAD;
        return INDEX_NULL_HEAD;
    }
    if(nodeId<0||neighbor<0){
        error_val=INDEX_INSERT_NEGATIVE_NODEID;
        return INDEX_INSERT_NEGATIVE_NODEID;
    }
    if(nodeId>hindex->size){
        error_val=INDEX_NODE_ID_OUT_BOUNDS;
        return INDEX_NODE_ID_OUT_BOUNDS;
    }
    //List doesn't exist
    if(hindex->index[nodeId].List_start==-1){
        ptr tmpptr;
        if((tmpptr=allocNewNode(hindex->buffer))<0){
            print_error();
            error_val=INDEX_ADD_EDGE_ALLOC_NEW_NODE;
            return INDEX_ADD_EDGE_ALLOC_NEW_NODE;
        }
        hindex->index[nodeId].List_start=tmpptr;
        hindex->index[nodeId].list_node_for_next_neigbor=tmpptr;
        hindex->index[nodeId].node_index_for_next_neighbor=0;
    }
    tmplnode=getListNode(hindex->buffer,hindex->index[nodeId].list_node_for_next_neigbor);
    if(tmplnode==NULL){
        print_error();
        error_val=INDEX_GET_LIST_NODE_FAIL;
        return INDEX_GET_LIST_NODE_FAIL;
    }
    tmplnode->neighbor[hindex->index[nodeId].node_index_for_next_neighbor]=neighbor;
    hindex->index[nodeId].node_index_for_next_neighbor++;
    if(hindex->index[nodeId].node_index_for_next_neighbor==N){
        ptr tmpptr;
        if((tmpptr=allocNewNode(hindex->buffer))<0){
            print_error();
            error_val=INDEX_ADD_EDGE_ALLOC_NEW_NODE;
            return INDEX_ADD_EDGE_ALLOC_NEW_NODE;
        }
        //changing nextptr in list's last node
        tmplnode=getListNode(hindex->buffer,hindex->index[nodeId].list_node_for_next_neigbor);
        if(tmplnode==NULL){
            print_error();
            error_val=INDEX_GET_LIST_NODE_FAIL;
            return INDEX_GET_LIST_NODE_FAIL;
        }
        tmplnode->nextListNode=tmpptr;
        //changing index ptr to last node
        hindex->index[nodeId].list_node_for_next_neigbor=tmpptr;
        //changing position in inner array in list node to 0
        hindex->index[nodeId].node_index_for_next_neighbor=0;
    }
    error_val=OK_SUCCESS;
    hindex->edges++;
    hindex->index[nodeId].edges++;
    return OK_SUCCESS;
}

rcode destroyNodeIndex(const Index_ptr hindex){
    if(hindex==NULL){
        error_val=INDEX_NULL_HEAD;
        return INDEX_NULL_HEAD;
    }
    free(hindex->index);
    destroyBuffer(hindex->buffer);
    free(hindex);
    error_val=OK_SUCCESS;
    return OK_SUCCESS;
}

pBuffer return_buffer(const Index_ptr hindex){
    if(hindex==NULL){
        error_val=INDEX_NULL_HEAD;
        return NULL;
    }
    error_val=OK_SUCCESS;
    return hindex->buffer;
}

int get_index_size(const Index_ptr hindex){
    if(hindex==NULL){
        error_val=INDEX_NULL_HEAD;
        return INDEX_NULL_HEAD;
    }
    error_val=OK_SUCCESS;
    return hindex->size;
}

int get_number_of_edges(const Index_ptr hindex){
    if(hindex==NULL){
        error_val=INDEX_NULL_HEAD;
        return INDEX_NULL_HEAD;
    }
    error_val=OK_SUCCESS;
    return hindex->edges;
}


int ret_biggest_node(const Index_ptr hindex){
    return hindex->nodes;
}
