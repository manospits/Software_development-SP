#include "buffer.h"
#include "error.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#define INIT_SIZE 10

extern  int error_val;


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
        error_val=BUFFER_CR_MALLOC;
        return NULL;
    }
    if((tmp->buffer=malloc(INIT_SIZE*sizeof(lnode)))==NULL){
         error_val=BUFFER_CR_SPACE_MALLOC;
         free(tmp);
         return NULL;
    }
    tmp->nodes=0;
    tmp->nextfreepos=0;
    tmp->size=INIT_SIZE;
    return tmp;
}

ptr allocNewNode(pBuffer buf){
    if(buf==NULL){
        error_val=NULL_BUFFER;
        return NULL_BUFFER;
    }
    lnode tmp;
    int i;
    ptr pos;
    for(i=0;i<N;i++){
        tmp.neighbor[i]=-1;
        tmp.edgeProperty[i]=-1;
    }
    tmp.nextListNode=-1;
    if(buf->nextfreepos >= buf->size*sizeof(lnode)){//THERE ISN'T SPACE FOR THE NEW NODE
        buf->buffer=realloc(buf->buffer,(buf->size*2)*sizeof(lnode));
        if(buf->buffer==NULL){
            error_val=BUFFER_DOUBLE_SIZE;
            return BUFFER_DOUBLE_SIZE;
        }
        buf->size*=2;
    }
    memcpy(buf->buffer+buf->nextfreepos,&tmp,sizeof(lnode));
    pos=buf->nextfreepos;
    buf->nextfreepos+=sizeof(lnode);
    buf->nodes++;
    return pos;
}

plnode getListNode(pBuffer buf,ptr pos){
    if(buf==NULL){
        error_val=NULL_BUFFER;
        return NULL;
    }
    if(pos<0 || pos >=buf->size *sizeof(lnode)){
        error_val=BUFFER_PTR_OUT_BOUNDS;
        return NULL;
    }
    return (plnode) (buf->buffer+pos);
}

rcode destroyBuffer(pBuffer buf){
    if(buf==NULL){
        return NULL_BUFFER;
    }
    if(buf->buffer==NULL){
         return NULL_BUFFER_SPACE;
    }
    free(buf->buffer);
    free(buf);
    return OK_SUCCESS;
}

