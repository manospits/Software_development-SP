#include "../include/error.h"
#include "../include/queries.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct qhead{
    int size;
    int front;
    int elements;
    query *array_queue;
}qhead;

qphead q_cr_list(){
    qphead tmphead;
    tmphead=malloc(sizeof(struct qhead));
    if(tmphead==NULL){
        print_errorv(LIST_CR_MALLOC);
        /*error_val=(LIST_CR_MALLOC);*/
        return NULL;
    }
    tmphead->size=QLIST_INIT_SIZE;
    tmphead->front=0;
    tmphead->elements=0;
    if((tmphead->array_queue=malloc(QLIST_INIT_SIZE*sizeof(struct query)))==NULL){
        print_errorv(LIST_CR_MALLOC);
        /*error_val=(LIST_CR_MALLOC);*/
        return NULL;
    }
    /*error_val=OK_SUCCESS;*/
    return tmphead;
}

rcode q_ds_list(qphead ltodestroy){
    if(ltodestroy==NULL){
        print_errorv(NULL_LIST);
        /*error_val=NULL_LIST;*/
        return NULL_LIST;
    }
    free(ltodestroy->array_queue);
    free (ltodestroy);
    /*error_val=OK_SUCCESS;*/
    return OK_SUCCESS;
}

rcode q_empty_list(qphead list_to_empty){
    if(list_to_empty==NULL){
        print_errorv(NULL_LIST);
        /*error_val=NULL_LIST;*/
        return NULL_LIST;
    }
    list_to_empty->front=0;
    list_to_empty->elements=0;
    /*error_val=OK_SUCCESS;*/
    return OK_SUCCESS;
}

rcode q_insert_back(qphead listh,uint32_t query_id,uint32_t nodea,uint32_t nodeb,long unsigned int version){
    if(listh==NULL){
        print_errorv(NULL_LIST);
        /*error_val=NULL_LIST;*/
        return NULL_LIST;
    }
    int pos,newsize;
    if(listh->elements+1>listh->size){
        newsize=listh->size*2;
        listh->array_queue=realloc(listh->array_queue,newsize*sizeof(struct query));
        if(listh->front!=0){
            memcpy(&listh->array_queue[newsize-(listh->size-listh->front)],&listh->array_queue[listh->front],(listh->size-listh->front)*sizeof(struct query));
            listh->front=newsize-(listh->size-listh->front);
        }
        listh->size=newsize;
    }
    pos=(listh->front+listh->elements < listh->size ) ? listh->front+listh->elements : listh->front+listh->elements-listh->size;
    listh->array_queue[pos].query_id=query_id;
    listh->array_queue[pos].nodea=nodea;
    listh->array_queue[pos].nodeb=nodeb;
    listh->array_queue[pos].version=version;
    listh->elements++;
    return OK_SUCCESS;
}

rcode q_pop_front(qphead listh){
    if(listh==NULL){
        print_errorv(NULL_LIST);
        /*error_val=NULL_LIST;*/
        return NULL_LIST;
    }
    if(listh->elements < 1){
        print_errorv(EMPTY_LIST);
        /*error_val=EMPTY_LIST;*/
        return EMPTY_LIST;
    }
    listh->front=(listh->front+1 <listh->size) ? listh->front+1 : 0;
    listh->elements--;
    /*error_val=OK_SUCCESS;*/
    return OK_SUCCESS;
}

int q_get_size(qphead listh){
    if(listh==NULL){
        print_errorv(NULL_LIST);
        /*error_val=NULL_LIST;*/
        return NULL_LIST;
    }
    error_val=OK_SUCCESS;
    return listh->elements;
}

qpnode q_peek(const qphead listh){
    if(listh==NULL){
        print_errorv(NULL_LIST);
        /*error_val=NULL_LIST;*/
        return NULL;
    }
    /*error_val=OK_SUCCESS;*/
    return &listh->array_queue[listh->front];
}

qpnode q_peek_back(const qphead listh){
    if(listh==NULL){
        print_errorv(NULL_LIST);
        /*error_val=NULL_LIST;*/
        return NULL;
    }
    /*error_val=OK_SUCCESS;*/
    int pos=(listh->front+listh->elements-1 <listh->size)? listh->front+listh->elements -1 : listh->front+listh->elements-1 -listh->size;
    return &listh->array_queue[pos];
}

