#include "error.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


typedef struct ghead{
    int size;
    int front;
    int elements;
    char *array_queue;
    rcode (*data_copy)(char*,void*);
    int (*data_cmp)(void*,void*);
    size_t data_size;

}ghead;

gphead gcr_list(size_t data_size,rcode(*data_copy)(char*,void*),int(*data_cmp)(void*,void*)){
    gphead tmphead;
    tmphead=malloc(sizeof(struct ghead));
    if(tmphead==NULL){
        error_val=(LIST_CR_MALLOC);
        return NULL;
    }
    tmphead->size=QUEUE_INIT_SIZE;
    tmphead->front=0;
    tmphead->elements=0;
    tmphead->data_copy=data_copy;
    tmphead->data_cmp=data_cmp;
    tmphead->data_size=data_size;
    if((tmphead->array_queue=malloc(QUEUE_INIT_SIZE*data_size))==NULL){
        error_val=(LIST_CR_MALLOC);
        return NULL;
    }
    error_val=OK_SUCCESS;
    return tmphead;
}

rcode gds_list(gphead ltodestroy){
    if(ltodestroy==NULL){
        error_val=NULL_LIST;
        return NULL_LIST;
    }
    free(ltodestroy->array_queue);
    free (ltodestroy);
    error_val=OK_SUCCESS;
    return OK_SUCCESS;
}

rcode gempty_list(gphead list_to_empty){
    if(list_to_empty==NULL){
        error_val=NULL_LIST;
        return NULL_LIST;
    }
    list_to_empty->front=0;
    list_to_empty->elements=0;
    error_val=OK_SUCCESS;
    return OK_SUCCESS;
}

rcode ginsert_back(gphead listh,void * data){
    if(listh==NULL){
        error_val=NULL_LIST;
        return NULL_LIST;
    }
    int pos,newsize;
    if(listh->elements+1>listh->size){
        newsize=listh->size*2;
        listh->array_queue=realloc(listh->array_queue,newsize*listh->data_size);
        if(listh->front!=0){
            memcpy(&listh->array_queue[listh->size],&listh->array_queue[0],(listh->front));
        }
        listh->front=newsize-(listh->size-listh->front);
        listh->size=newsize;
    }
    pos=(((listh->front/listh->data_size)+listh->elements)%listh->size)*listh->data_size;
    listh->data_copy(&listh->array_queue[pos],data);
    listh->elements++;
    return OK_SUCCESS;
}

rcode gpop_front(gphead listh){
    if(listh==NULL){
        error_val=NULL_LIST;
        return NULL_LIST;
    }
    if(listh->elements < 1){
        error_val=EMPTY_LIST;
        return EMPTY_LIST;
    }
    listh->front=((listh->front+listh->data_size)%listh->size)*listh->data_size;
    listh->elements--;
    error_val=OK_SUCCESS;
    return OK_SUCCESS;
}

rcode gpop_back(gphead listh){
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

int gget_size(gphead listh){
    if(listh==NULL){
        error_val=NULL_LIST;
        return NULL_LIST;
    }
    error_val=OK_SUCCESS;
    return listh->elements;
}

void* gpeek(const gphead listh){
    if(listh==NULL){
        error_val=NULL_LIST;
        return NULL;
    }
    error_val=OK_SUCCESS;
    return &listh->array_queue[listh->front];
}

rcode int_copy(char*a,void*b){
    *(int*)a=*(int*)b;
    return OK_SUCCESS;
}

int int_cmp(void*a,void*b){
    if(*(int*)a==*(int*)b){
        return 0;
    }
    else if(*(int*)a<*(int*)b){
        return -1;
    }
    else{
        return 1;
    }
}
