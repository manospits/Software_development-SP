#include "error.h"
#include "struct_list.h"
#include "intlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct sthead{
    int size;
    int front;
    int elements;
    stnode *array_queue;
}sthead;

stphead st_cr_list(){
    stphead tmphead;
    tmphead=malloc(sizeof(struct sthead));
    if(tmphead==NULL){
        error_val=(LIST_CR_MALLOC);
        return NULL;
    }
    tmphead->size=STQUEUE_INIT_SIZE;
    tmphead->front=0;
    tmphead->elements=0;
    if((tmphead->array_queue=malloc(STQUEUE_INIT_SIZE*sizeof(struct stnode)))==NULL){
        error_val=(LIST_CR_MALLOC);
        return NULL;
    }
    error_val=OK_SUCCESS;
    return tmphead;
}

rcode st_ds_list(stphead ltodestroy){
    if(ltodestroy==NULL){
        error_val=NULL_LIST;
        return NULL_LIST;
    }
    free(ltodestroy->array_queue);
    free (ltodestroy);
    error_val=OK_SUCCESS;
    return OK_SUCCESS;
}

rcode st_empty_list(stphead list_to_empty){
    if(list_to_empty==NULL){
        error_val=NULL_LIST;
        return NULL_LIST;
    }
    list_to_empty->front=0;
    list_to_empty->elements=0;
    error_val=OK_SUCCESS;
    return OK_SUCCESS;
}

rcode st_insert_back(stphead listh,uint32_t data,uint32_t tag){
    if(listh==NULL){
        error_val=NULL_LIST;
        return NULL_LIST;
    }
    int pos,newsize;
    if(listh->elements+1>listh->size){
        newsize=listh->size*2;
        listh->array_queue=realloc(listh->array_queue,newsize*sizeof(struct stnode));
        if(listh->front!=0){
            memcpy(&listh->array_queue[newsize-(listh->size-listh->front)],&listh->array_queue[listh->front],(listh->size-listh->front)*sizeof(struct stnode));
            listh->front=newsize-(listh->size-listh->front);
        }
        listh->size=newsize;
    }
    pos=(listh->front+listh->elements)%listh->size;
    listh->array_queue[pos].data=data;
    listh->array_queue[pos].tag=tag;
    listh->elements++;
    return OK_SUCCESS;
}

rcode st_pop_front(stphead listh){
    if(listh==NULL){
        error_val=NULL_LIST;
        return NULL_LIST;
    }
    if(listh->elements < 1){
        error_val=EMPTY_LIST;
        return EMPTY_LIST;
    }
    listh->front=(listh->front+1)%listh->size;
    listh->elements--;
    error_val=OK_SUCCESS;
    return OK_SUCCESS;
}

int st_get_size(stphead listh){
    if(listh==NULL){
        error_val=NULL_LIST;
        return NULL_LIST;
    }
    error_val=OK_SUCCESS;
    return listh->elements;
}

int st_peek(const stphead listh){
    if(listh==NULL){
        error_val=NULL_LIST;
        return NULL_LIST;
    }
    error_val=OK_SUCCESS;
    return listh->array_queue[listh->front].data;
}

int st_peek_back(const stphead listh){
    if(listh==NULL){
        error_val=NULL_LIST;
        return NULL_LIST;
    }
    error_val=OK_SUCCESS;
    int pos=(listh->front+listh->elements-1)%listh->size;
    return listh->array_queue[pos].data;
}

int st_get_tag(const stphead listh,uint32_t data){
    if(listh==NULL){
        error_val=NULL_LIST;
        return NULL_LIST;
    }
    int i,pos;
    for(i=0;i<listh->elements;i++){
        pos=(listh->front+i)%listh->size;
        if(listh->array_queue[pos].data==data){
            error_val=OK_SUCCESS;
            return listh->array_queue[pos].tag;
        }
    }
    error_val=NODE_MISSING;
    return NODE_MISSING;
}

int st_in(const stphead listh,uint32_t data){
    int i,pos=listh->front;
    for(i=0;i<listh->elements;i++){
        if(listh->array_queue[pos].data==data){
            return 1;
        }
        pos++;
        if(pos==listh->size){
            pos=0;
        }
    }
    return 0;
}

iterator st_ret_iterator(const stphead listh){
    if(listh->size==0){
        return -1;
    }
    else{
        return listh->front;
    }
}

stpnode st_get_iterator_data(const stphead listh,iterator it){
    return &listh->array_queue[it];
}

int st_advance_iterator(const stphead listh,iterator it){
    if(it==((listh->front+listh->elements-1 <listh->size)? listh->front+listh->elements -1 : listh->front+listh->elements-1 -listh->size)){
        return -1;
    }
    else{
        return (it+1)%listh->size;
    }
}
