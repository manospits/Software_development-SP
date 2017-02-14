#include "../include/buffer.h"
#include "../include/error.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#define INIT_SIZE 20000

typedef struct Buffer{
    char *buffer;
    int nodes;
    int nextfreepos;
    int size;
}Buffer;

pBuffer createBuffer(){
    pBuffer tmp;
    tmp=malloc(sizeof(Buffer));
    if(tmp==NULL){
        /*error_val=BUFFER_CR_MALLOC;*/
        print_errorv(BUFFER_CR_MALLOC);
        return NULL;
    }
    if((tmp->buffer=malloc(INIT_SIZE*sizeof(lnode)))==NULL){
        print_errorv(BUFFER_CR_SPACE_MALLOC);
        /*error_val=BUFFER_CR_SPACE_MALLOC;*/
        free(tmp);
        return NULL;
    }
    tmp->nodes=0;
    tmp->nextfreepos=0;
    tmp->size=INIT_SIZE;
    /*error_val=OK_SUCCESS;*/
    return tmp;
}

ptr allocNewNode(pBuffer buf){
    if(buf==NULL){
        /*error_val=NULL_BUFFER;*/
        print_errorv(NULL_BUFFER);
        return NULL_BUFFER;
    }
    plnode tmp;
    int i;
    ptr pos;
    if(buf->nextfreepos >= buf->size*sizeof(lnode)){//THERE ISN'T SPACE FOR THE NEW NODE
        buf->buffer=realloc(buf->buffer,(buf->size*2)*sizeof(lnode));
        if(buf->buffer==NULL){
            print_errorv(BUFFER_DOUBLE_SIZE);
            /*error_val=BUFFER_DOUBLE_SIZE;*/
            return BUFFER_DOUBLE_SIZE;
        }
        buf->size<<=1;
    }
    tmp=(plnode)(buf->buffer+buf->nextfreepos);
    for(i=0;i<N;i++){
        tmp->neighbor[i]=-1;
        tmp->edgeProperty[i]=-1;
    }
    tmp->nextListNode=-1;
    pos=buf->nextfreepos;
    buf->nextfreepos+=sizeof(lnode);
    buf->nodes++;
    /*error_val=OK_SUCCESS;*/
    return pos;
}

plnode getListNode(pBuffer buf,ptr pos){
    if(buf==NULL){
        print_errorv(NULL_BUFFER);
        /*error_val=NULL_BUFFER;*/
        return NULL;
    }
    if(pos<0 || pos >=buf->size *sizeof(lnode)){
        print_errorv(BUFFER_PTR_OUT_BOUNDS);
        /*error_val=BUFFER_PTR_OUT_BOUNDS;*/
        return NULL;
    }
    /*error_val=OK_SUCCESS;*/
    return (plnode) (buf->buffer+pos);
}

rcode destroyBuffer(pBuffer buf){
    if(buf==NULL){
        print_errorv(NULL_BUFFER);
        return NULL_BUFFER;
    }
    if(buf->buffer==NULL){
        print_errorv(NULL_BUFFER_SPACE);
         return NULL_BUFFER_SPACE;
    }
    free(buf->buffer);
    free(buf);
    /*error_val=OK_SUCCESS;*/
    return OK_SUCCESS;
}

