#include "error.h"
#include "visited.h"
#include <stdint.h>
#include <stdlib.h>

typedef struct visited_node{
    int mark;
    int tag;
    int expanded;
}visited_node;

typedef struct visited_head{
    visited_node * nodes;
    uint32_t size;
    uint32_t loop_count;
}visited_head;

pvis create_visited(int size){
    pvis a;
    int i;
    if(size<=0){
        /*error_val=VISITED_WRONG_PARAMTERS;*/
        return NULL;
    }
    if((a=malloc(sizeof(struct visited_head)))==NULL){
        /*error_val=VISITED_MALLOC_ERROR;*/
        return NULL;
    }
    if((a->nodes=malloc(size*sizeof(struct visited_node)))==NULL){
        /*error_val=VISITED_MALLOC_ERROR;*/
        return NULL;
    }
    a->size=size;
    a->loop_count=0;
    for(i=0;i<size;i++){
        a->nodes[i].mark=-1;
        a->nodes[i].tag=-1;
        a->nodes[i].expanded=-1;
    }
    return a;
}

rcode v_mark(pvis a,uint32_t data,int tag,int expanded){
    if(data >= a->size || (tag !=1&&tag!=0)||(expanded!=0&&expanded!=1)){
        //TODO
    }
    a->nodes[data].mark=a->loop_count;
    a->nodes[data].tag=tag;
    a->nodes[data].expanded=expanded;
    return OK_SUCCESS;

}

int v_visited(pvis a,uint32_t data){
    if (a->nodes[data].mark==a->loop_count){
        return 1 ;
    }
    else{
        return 0;
        }
}

rcode v_update_loop(pvis a,uint32_t size){
    if(size>a->size){
        uint32_t new_size=size,i;
        while(new_size<=size){
            new_size<<=1;
        }
        a->nodes=realloc(a->nodes,(new_size*(sizeof(struct visited_node))));
        for(i=a->size;i<new_size;i++){
            a->nodes[i].mark=-1;
            a->nodes[i].tag=-1;
            a->nodes[i].expanded=-1;
        }
        a->size=new_size;
    }
    a->loop_count++;
    return OK_SUCCESS;
}

rcode v_ds_visited(pvis a){
    free(a->nodes);
    free(a);
    return OK_SUCCESS;
}

int v_ret_tag(pvis a,uint32_t data){
    return a->nodes[data].tag;
}

int v_ret_expanded(pvis a,uint32_t data){
    return a->nodes[data].expanded;
}

rcode v_set_expanded(pvis a,uint32_t data,int expanded){
    a->nodes[data].expanded=expanded;
    return OK_SUCCESS;
}

