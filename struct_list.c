#include "error.h"
#include "struct_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct stnode {
    uint32_t data;
    uint32_t tag;
    struct stnode* next;
} stnode;

typedef struct sthead{
    int size;
    stpnode front;
    stpnode end;
}sthead;

stpnode st_cr_node(uint32_t data,uint32_t tag);
rcode st_ds_node(stpnode node_to_destroy);

stphead st_cr_list(){
    stphead tmphead;
    tmphead=malloc(sizeof(struct sthead));
    if(tmphead==NULL){
        error_val=(LIST_CR_MALLOC);
        return NULL;
    }
    tmphead->size=0;
    tmphead->front=NULL;
    tmphead->end=NULL;
    error_val=OK_SUCCESS;
    return tmphead;
}

stpnode st_cr_node(uint32_t data,uint32_t tag){
    stnode* tmpnode;
    tmpnode=malloc(sizeof(struct stnode));
    if(tmpnode==NULL){
        error_val=(LISTNODE_CR_MALLOC);
        return NULL;
    }
    tmpnode->data=data;
    tmpnode->tag=tag;
    tmpnode->next=NULL;
    error_val=OK_SUCCESS;
    return tmpnode;
}

rcode st_ds_node(stpnode node_to_destroy){
    if(node_to_destroy==NULL){
        error_val=NULL_NODE;
        return NULL_NODE;
    }
    free(node_to_destroy);
    error_val=OK_SUCCESS;
    return OK_SUCCESS;
}

rcode st_ds_list(stphead ltodestroy){
    if(ltodestroy==NULL){
        error_val=NULL_LIST;
        return NULL_LIST;
    }
    stpnode todel;
    stpnode next=ltodestroy->front;
    while(next!=NULL){
        todel=next;
        next=next->next;
        st_ds_node(todel);
    }
    free (ltodestroy);
    error_val=OK_SUCCESS;
    return OK_SUCCESS;
}

rcode st_empty_list(stphead list_to_empty){
    if(list_to_empty==NULL){
        error_val=NULL_LIST;
        return NULL_LIST;
    }
    stpnode todel;
    stpnode next=list_to_empty->front;
    while(next!=NULL){
        todel=next;
        next=next->next;
        st_ds_node(todel);
    }
    list_to_empty->size=0;
    list_to_empty->front=NULL;
    list_to_empty->end=NULL;
    error_val=OK_SUCCESS;
    return OK_SUCCESS;
}

rcode st_insert(stphead listh,uint32_t data,uint32_t tag){
    if(listh==NULL){
        error_val=NULL_LIST;
        return NULL_LIST;
    }
    stpnode nptr=st_cr_node(data,tag);
    if(listh->size==0){
        listh->front=nptr;
        listh->end=nptr;
        listh->size++;
    }
    else{
        nptr->next=listh->front;
        listh->front=nptr;
        listh->size++;
    }
    error_val=OK_SUCCESS;
    return OK_SUCCESS;
}

rcode st_insert_sorted(stphead listh,uint32_t data,uint32_t tag){
    if(listh==NULL){
        error_val=NULL_LIST;
        return NULL_LIST;
    }
    stpnode nptr=st_cr_node(data,tag);
    stpnode pr_node,node,temp2;
    node=pr_node=listh->front;
    int i=0;
    for(i=0;i<listh->size;i++){
        if(data<node->data){
            pr_node=node;
            break;
        }
        else{
            temp2=node;
            node=node->next;
        }
    }
    if(listh->size==0){
        listh->front=nptr;
        listh->end=nptr;
    }
    else{
        if(pr_node==listh->front && i!=listh->size){//front
            listh->front=nptr;
            nptr->next=pr_node;
        }
        else if(i==listh->size){//end
            listh->end->next=nptr;
            listh->end=nptr;
        }
        else{
            temp2->next=nptr;
            nptr->next=pr_node;
        }
    }
    listh->size++;
    error_val=OK_SUCCESS;
    return OK_SUCCESS;
}

rcode st_insert_back(stphead listh,uint32_t data,uint32_t tag){
    if(listh==NULL){
        error_val=NULL_LIST;
        return NULL_LIST;
    }
    stpnode nptr=st_cr_node(data,tag);
    if(listh->size==0){
        listh->front=nptr;
        listh->end=nptr;
        listh->size++;
    }
    else{
        listh->end->next=nptr;
        listh->end=nptr;
        listh->size++;
    }
    error_val=OK_SUCCESS;
    return OK_SUCCESS;
}

rcode st_delete(stphead listh,uint32_t data){
    /*puts("delete call");*/
    if(listh==NULL){
        error_val=NULL_LIST;
        return NULL_LIST;
    }
    if(listh->size==1 && data==listh->front->data){
        st_ds_node(listh->front);
        listh->front=NULL;
        listh->end=NULL;
        listh->size=0;
    }
    else if (listh->size > 1){
        stpnode pr_node=listh->front;
        stpnode todel=listh->front;
        stpnode next;
        while(todel!=NULL){
            if(todel->data==data){
                if(listh->size==1){
                    listh->end=NULL;
                    listh->front=NULL;
                }
                else if(listh->front==todel){
                    listh->front=todel->next;
                }
                else if(listh->end==todel){
                    listh->end=pr_node;
                    pr_node->next=NULL;
                }
                else{
                    pr_node->next=todel->next;
                }
                next=todel->next;
                st_ds_node(todel);
                todel=next;
                listh->size--;
            }
            else{
                pr_node=todel;
                todel=todel->next;
            }
        }
    }
    error_val=OK_SUCCESS;
    return OK_SUCCESS;
}

rcode st_pop_front(stphead listh){
    if(listh==NULL){
        error_val=NULL_LIST;
        return NULL_LIST;
    }
    if(listh->size < 1){
        error_val=EMPTY_LIST;
        return EMPTY_LIST;
    }
    if(listh->size==1){
        st_ds_node(listh->front);
        listh->front=NULL;
        listh->end=NULL;
        listh->size=0;
    }
    else{
        stpnode todel=listh->front;
        listh->front=listh->front->next;
        st_ds_node(todel);
        listh->size--;
    }
    error_val=OK_SUCCESS;
    return OK_SUCCESS;
}

rcode st_pop_back(stphead listh){
    if(listh==NULL){
        error_val=NULL_LIST;
        return NULL_LIST;
    }
    if(listh->size < 1){
        error_val=EMPTY_LIST;
        return EMPTY_LIST;
    }
    if(listh->size==1){
        st_ds_node(listh->front);
        listh->front=NULL;
        listh->end=NULL;
        listh->size=0;
    }
    else{
        stpnode todel=listh->front;
        stpnode pr_node=listh->front;
        while(todel!=listh->end){
            pr_node=todel;
            todel=todel->next;
        }
        listh->end=pr_node;
        pr_node->next=NULL;

        st_ds_node(todel);
        listh->size--;
    }
    error_val=OK_SUCCESS;
    return OK_SUCCESS;
}

int st_in(stphead listh,uint32_t data){
    if(listh==NULL){
        error_val=NULL_LIST;
        return NULL_LIST;
    }
    stpnode tmp=listh->front;
    while(tmp!=NULL){
        if(data==tmp->data){
            error_val=OK_SUCCESS;
            return 1;
        }
        tmp=tmp->next;
    }
    error_val=OK_SUCCESS;
    return 0;
}

int st_ins(stphead listh,uint32_t data){
    if(listh==NULL){
        error_val=NULL_LIST;
        return NULL_LIST;
    }
    stpnode tmp=listh->front;
    while(tmp!=NULL && tmp->data<=data){
        if(data==tmp->data){
            error_val=OK_SUCCESS;
            return 1;
        }
        tmp=tmp->next;
    }
    error_val=OK_SUCCESS;
    return 0;
}

int st_get_size(stphead listh){
    if(listh==NULL){
        error_val=NULL_LIST;
        return NULL_LIST;
    }
    error_val=OK_SUCCESS;
    return listh->size;
}

int st_peek(const stphead listh){
    if(listh==NULL){
        error_val=NULL_LIST;
        return NULL_LIST;
    }
    else{
        error_val=OK_SUCCESS;
        return listh->front->data;
    }
}

int st_peekback(const stphead listh){
    if(listh==NULL){
        error_val=NULL_LIST;
        return NULL_LIST;
    }
    else{
        error_val=OK_SUCCESS;
        return listh->end->data;
    }
}

int st_get_data(stpnode nd){
    if(nd==NULL){
        error_val=NULL_NODE;
        return NULL_LIST;
    }
    else{
        error_val=OK_SUCCESS;
        return nd->data;
    }
}

stpnode st_get_list(const stphead listh){
    if(listh==NULL){
        error_val=NULL_LIST;
        return NULL;
    }
    else{
        error_val=OK_SUCCESS;
        return listh->front;
    }
}

stpnode st_next_node(const stpnode nd){
    if(nd==NULL){
        return NULL;
    }
    else{
        error_val=OK_SUCCESS;
        return nd->next;
    }
}

int st_get_tag(const stphead listh,uint32_t data){
    if(listh==NULL){
        error_val=NULL_LIST;
        return NULL_LIST;
    }
    stpnode tmp=listh->front;
    while(tmp!=NULL){
        if(data==tmp->data){
            error_val=OK_SUCCESS;
            return tmp->tag;
        }
        tmp=tmp->next;
    }
    error_val=NODE_MISSING;
    return NODE_MISSING;
}
