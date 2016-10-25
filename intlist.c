#include "error.h"
#include "intlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

extern int error_val;

typedef struct node {
    uint32_t data;
    struct node* next;
} node;

typedef struct head{
    int size;
    pnode front;
    pnode end;
}head;

phead cr_list(){
    phead tmphead;
    tmphead=malloc(sizeof(struct head));
    if(tmphead==NULL){
        error_val=(LIST_CR_MALLOC);
        return NULL;
    }
    tmphead->size=0;
    tmphead->front=NULL;
    tmphead->end=NULL;
    return tmphead;
}

pnode cr_node(uint32_t data){
    node* tmpnode;
    tmpnode=malloc(sizeof(struct node));
    if(tmpnode==NULL){
        error_val=(LISTNODE_CR_MALLOC);
        return NULL;
    }
    tmpnode->data=data;
    tmpnode->next=NULL;
    return tmpnode;
}

rcode ds_node(pnode node_to_destroy){
    if(node_to_destroy==NULL){
        error_val=NULL_NODE;
        return NULL_NODE;
    }
    free(node_to_destroy);
    return OK_SUCCESS;
}

rcode ds_list(phead ltodestroy){
    if(ltodestroy==NULL){
        error_val=NULL_LIST;
        return NULL_LIST;
    }
    pnode todel;
    pnode next=ltodestroy->front;
    while(next!=NULL){
        todel=next;
        next=next->next;
        ds_node(todel);
    }
    free (ltodestroy);
    return OK_SUCCESS;
}

rcode insert(phead listh,uint32_t data){
    if(listh==NULL){
        error_val=NULL_LIST;
        return NULL_LIST;
    }
    pnode nptr=cr_node(data);
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
    return OK_SUCCESS;
}

rcode insert_sorted(phead listh,uint32_t data){
    if(listh==NULL){
        error_val=NULL_LIST;
        return NULL_LIST;
    }
    pnode nptr=cr_node(data);
    pnode prev,node,temp2;
    node=prev=listh->front;
    int i=0;
    for(i=0;i<listh->size;i++){
        if(data<node->data){
            prev=node;
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
        if(prev==listh->front && i!=listh->size){//front
            listh->front=nptr;
            nptr->next=prev;
        }
        else if(i==listh->size){//end
            listh->end->next=nptr;
            listh->end=nptr;
        }
        else{
            temp2->next=nptr;
            nptr->next=prev;
        }
    }
    listh->size++;
    return OK_SUCCESS;
}

rcode insert_back(phead listh,uint32_t data){
    if(listh==NULL){
        error_val=NULL_LIST;
        return NULL_LIST;
    }
    pnode nptr=cr_node(data);
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
    return OK_SUCCESS;
}

rcode delete(phead listh,uint32_t data){
    /*puts("delete call");*/
    if(listh==NULL){
        error_val=NULL_LIST;
        return NULL_LIST;
    }
    if(listh->size==1 && data==listh->front->data){
        ds_node(listh->front);
        listh->front=NULL;
        listh->end=NULL;
        listh->size=0;
    }
    else if (listh->size > 1){
        pnode prev=listh->front;
        pnode todel=listh->front;
        pnode next;
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
                    listh->end=prev;
                    prev->next=NULL;
                }
                else{
                    prev->next=todel->next;
                }
                next=todel->next;
                ds_node(todel);
                todel=next;
                listh->size--;
            }
            else{
                prev=todel;
                todel=todel->next;
            }
        }
    }
    return OK_SUCCESS;
}

rcode pop_back(phead listh){
    if(listh==NULL){
        error_val=NULL_LIST;
        return NULL_LIST;
    }
    if(listh->size < 1){
        return OK_SUCCESS;
    }
    if(listh->size==1){
        ds_node(listh->front);
        listh->front=NULL;
        listh->end=NULL;
        listh->size=0;
    }
    else{
        pnode todel=listh->front;
        pnode prev=listh->front;
        while(todel!=listh->end){
            prev=todel;
            todel=todel->next;
        }
        listh->end=prev;
        prev->next=NULL;

        ds_node(todel);
        listh->size--;
    }
    return OK_SUCCESS;
}

int in(phead listh,uint32_t data){
    if(listh==NULL){
        error_val=NULL_LIST;
        return NULL_LIST;
    }
    pnode tmp=listh->front;
    while(tmp!=NULL){
        if(data==tmp->data){
            return 1;
        }
        tmp=tmp->next;
    }
    return 0;
}

int ins(phead listh,uint32_t data){
    if(listh==NULL){
        error_val=NULL_LIST;
        return NULL_LIST;
    }
    pnode tmp=listh->front;
    while(tmp!=NULL && tmp->data<=data){
        if(data==tmp->data){
            return 1;
        }
        tmp=tmp->next;
    }
    return 0;
}

int get_size(phead listh){
    if(listh==NULL){
        error_val=NULL_LIST;
        return NULL_LIST;
    }
    return listh->size;
}

int peek(const phead listh){
    if(listh==NULL){
        error_val=NULL_LIST;
        return NULL_LIST;
    }
    else{
        return listh->front->data;
    }
}

int peekback(const phead listh){
    if(listh==NULL){
        error_val=NULL_LIST;
        return NULL_LIST;
    }
    else{
        return listh->end->data;
    }
}

int get_data(pnode nd){
    if(nd==NULL){
        error_val=NULL_NODE;
        return NULL_LIST;
    }
    else{
        return nd->data;
    }
}

pnode get_list(const phead listh){
    if(listh==NULL){
        error_val=NULL_LIST;
        return NULL;
    }
    else{
        return listh->front;
    }
}

pnode next_node(const pnode nd){
    if(nd==NULL){
        return NULL;
    }
    else{
        return nd->next;
    }
}
