#include "../include/error.h"
#include "../include/intlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct head{
    long int size;
    uint32_t *array_queue;
    int front;
    int elements;
}head;

phead cr_list(){
    phead tmphead;
    tmphead=malloc(sizeof(struct head));
    if(tmphead==NULL){
        print_errorv(LIST_CR_MALLOC);
        /*error_val=(LIST_CR_MALLOC);*/
        return NULL;
    }
    tmphead->size=QUEUE_INIT_SIZE;
    tmphead->front=0;
    tmphead->elements=0;
    if((tmphead->array_queue=malloc(QUEUE_INIT_SIZE*sizeof(uint32_t)))==NULL){
        print_errorv(LIST_CR_MALLOC);
        /*error_val=(LIST_CR_MALLOC);*/
        return NULL;
    }
    /*error_val=OK_SUCCESS;*/
    return tmphead;
}

rcode ds_list(phead ltodestroy){
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

rcode empty_list(phead list_to_empty){
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

rcode insert_back(phead listh,uint32_t data){

    if(listh==NULL){
        print_errorv(NULL_LIST);
        /*error_val=NULL_LIST;*/
        return NULL_LIST;
    }
    int pos,newsize;
    if(listh->elements+1>listh->size){
        newsize=listh->size*2;
        listh->array_queue=realloc(listh->array_queue,newsize*sizeof(uint32_t));
        if(listh->front!=0){
            memcpy(&(listh->array_queue[newsize-(listh->size-listh->front)]),&(listh->array_queue[listh->front]),(listh->size-listh->front)*sizeof(uint32_t));
            listh->front=newsize-(listh->size-listh->front);
        }
        listh->size=newsize;
    }
    pos=(listh->front+listh->elements < listh->size ) ? listh->front+listh->elements : listh->front+listh->elements-listh->size;
    /*if(listh->front==pos)*/
    /*printf("front %d pos %d, %ld %d\n",listh->front,pos,listh->size,listh->elements);*/
    listh->array_queue[pos]=data;
    listh->elements++;
    /*puts("insert_back_ok");*/
    return OK_SUCCESS;
}

rcode pop_front(phead listh){
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

uint32_t peek_pop_front(phead listh){
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
    uint32_t tmp;
    tmp=listh->array_queue[listh->front];
    listh->front=(listh->front+1 <listh->size) ? listh->front+1 : 0;
    listh->elements--;
    /*error_val=OK_SUCCESS;*/
    return tmp;
}
rcode pop_back(phead listh){
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
    listh->elements--;
    /*error_val=OK_SUCCESS;*/
    return OK_SUCCESS;
}

int get_size(phead listh){
    if(listh==NULL){
        print_errorv(NULL_LIST);
        /*error_val=NULL_LIST;*/
        return NULL_LIST;
    }
    /*error_val=OK_SUCCESS;*/
    return listh->elements;
}

int* get_sizep(phead listh){
    if(listh==NULL){
        print_errorv(NULL_LIST);
        /*error_val=NULL_LIST;*/
        return NULL;
    }
    /*error_val=OK_SUCCESS;*/
    return &listh->elements;
}

int peek(const phead listh){
    if(listh==NULL){
        print_errorv(NULL_LIST);
        /*error_val=NULL_LIST;*/
        return NULL_LIST;
    }
    /*error_val=OK_SUCCESS;*/
    return listh->array_queue[listh->front];
}

int peek_back(const phead listh){
    if(listh==NULL){
        print_errorv(NULL_LIST);
        /*error_val=NULL_LIST;*/
        return NULL_LIST;
    }
    /*error_val=OK_SUCCESS;*/
    int pos=(listh->front+listh->elements-1 <listh->size)? listh->front+listh->elements -1 : listh->front+listh->elements-1 -listh->size;
    return listh->array_queue[pos];
}

iterator ret_iterator(const phead listh){
    if(listh==NULL){
        print_errorv(NULL_LIST);
        /*error_val=NULL_LIST;*/
        return NULL_LIST;
    }
    if(listh->size==0){
        return -1;
    }
    else{
        return listh->front;
    }
}

int get_iterator_data(const phead listh,iterator it){
    return listh->array_queue[it];
}

int advance_iterator(const phead listh,iterator it){
    if(listh==NULL){
        print_errorv(NULL_LIST);
        /*error_val=NULL_LIST;*/
        return NULL_LIST;
    }
    if(it== ((listh->front+listh->elements-1 <listh->size)? listh->front+listh->elements -1 : listh->front+listh->elements-1 -listh->size)){
        return -1;
    }
    else{
        return (it+1 <listh->size) ? it+1 : 0;
    }
}

int in(const phead listh,uint32_t data){
    int i,pos=listh->front;
    if(listh==NULL){
        print_errorv(NULL_LIST);
        /*error_val=NULL_LIST;*/
        return NULL_LIST;
    }
    for(i=0;i<listh->elements;i++){
        if(listh->array_queue[pos]==data){
            return 1;
        }
        pos++;
        if(pos==listh->size){
            pos=0;
        }
    }
    return 0;
}
