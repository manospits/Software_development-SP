#include "error.h"
#include "intlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct node {
    uint32_t data;
} node;

typedef struct head{
    long int size;
    int front;
    int elements;
    node *array_queue;
}head;

phead cr_list(){
    phead tmphead;
    tmphead=malloc(sizeof(struct head));
    if(tmphead==NULL){
        error_val=(LIST_CR_MALLOC);
        return NULL;
    }
    tmphead->size=QUEUE_INIT_SIZE;
    tmphead->front=0;
    tmphead->elements=0;
    if((tmphead->array_queue=malloc(QUEUE_INIT_SIZE*sizeof(struct node)))==NULL){
        error_val=(LIST_CR_MALLOC);
        return NULL;
    }
    error_val=OK_SUCCESS;
    return tmphead;
}

rcode ds_list(phead ltodestroy){
    if(ltodestroy==NULL){
        error_val=NULL_LIST;
        return NULL_LIST;
    }
    free(ltodestroy->array_queue);
    free (ltodestroy);
    error_val=OK_SUCCESS;
    return OK_SUCCESS;
}

rcode empty_list(phead list_to_empty){
    if(list_to_empty==NULL){
        error_val=NULL_LIST;
        return NULL_LIST;
    }
    list_to_empty->front=0;
    list_to_empty->elements=0;
    error_val=OK_SUCCESS;
    return OK_SUCCESS;
}

rcode insert_back(phead listh,uint32_t data){
    /*puts("insert_back");*/
    if(listh==NULL){
        error_val=NULL_LIST;
        return NULL_LIST;
    }
    int pos,newsize;
    if(listh->elements+1>listh->size){
        newsize=listh->size*2;
        listh->array_queue=realloc(listh->array_queue,newsize*sizeof(struct node));
        if(listh->front!=0){
            memcpy(&(listh->array_queue[newsize-(listh->size-listh->front)]),&(listh->array_queue[listh->front]),(listh->size-listh->front)*sizeof(struct node));
            listh->front=newsize-(listh->size-listh->front);
        }
        listh->size=newsize;
    }
    pos=(listh->front+listh->elements)%listh->size;
    /*if(listh->front==pos)*/
        /*printf("front %d pos %d, %ld %d\n",listh->front,pos,listh->size,listh->elements);*/
    listh->array_queue[pos].data=data;
    listh->elements++;
    /*puts("insert_back_ok");*/
    return OK_SUCCESS;
}

rcode pop_front(phead listh){
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

rcode pop_back(phead listh){
    if(listh==NULL){
        error_val=NULL_LIST;
        return NULL_LIST;
    }
    if(listh->elements < 1){
        error_val=EMPTY_LIST;
        return EMPTY_LIST;
    }
    listh->elements--;
    error_val=OK_SUCCESS;
    return OK_SUCCESS;
}

int get_size(phead listh){
    if(listh==NULL){
        error_val=NULL_LIST;
        return NULL_LIST;
    }
    error_val=OK_SUCCESS;
    return listh->elements;
}

int peek(const phead listh){
    if(listh==NULL){
        error_val=NULL_LIST;
        return NULL_LIST;
    }
    error_val=OK_SUCCESS;
    return listh->array_queue[listh->front].data;
}

int peek_back(const phead listh){
    if(listh==NULL){
        error_val=NULL_LIST;
        return NULL_LIST;
    }
    error_val=OK_SUCCESS;
    int pos=(listh->front+listh->elements-1)%listh->size;
    return listh->array_queue[pos].data;
}
iterator ret_iterator(const phead listh){
    return listh->front;
}

int get_iterator_data(const phead listh,iterator it){
    return listh->array_queue[it].data;
}

int advance_iterator(const phead listh,iterator it){
    if(it==(listh->front+listh->elements-1)%listh->size){
        return -1;
    }
    else{
        return (it+1)%listh->size;
    }
}

int in(const phead listh,uint32_t data){
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
