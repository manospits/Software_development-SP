#include "intlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    int data;
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
    tmphead->size=0;
    tmphead->front=NULL;
    tmphead->end=NULL;
    return tmphead;
}

pnode cr_node(int data){
    node* tmpnode;
    tmpnode=malloc(sizeof(struct node));
    tmpnode->data=data;
    tmpnode->next=NULL;
    return tmpnode;
}

void ds_node(pnode node_to_destroy){
    free(node_to_destroy);
}

void ds_list(phead ltodestroy){
    if(ltodestroy==NULL){
        return;
    }
    pnode todel;
    pnode next=ltodestroy->front;
    while(next!=NULL){
        todel=next;
        next=next->next;
        ds_node(todel);
    }
    free (ltodestroy);
}

void insert(phead listh,int data){
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
}

void insert_sorted(phead listh,int data){
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
}

void insert_back(phead listh,int data){
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
}

void delete(phead listh,int data){
    /*puts("delete call");*/
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
}

int pop_back(phead listh){
    if(listh->size < 1){
        return 1;
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
    return 0;
}

int in(phead listh,int data){
    pnode tmp=listh->front;
    while(tmp!=NULL){
        if(data==tmp->data){
            return 1;
        }
        tmp=tmp->next;
    }
    return 0;
}

int ins(phead listh,int data){
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
    return listh->size;
}

int peek(const phead listh){
    if(listh==NULL){
        return -1;
    }
    else{
        return listh->front->data;
    }
}

int peekback(const phead listh){
    if(listh==NULL){
        return -1;
    }
    else{
        return listh->end->data;
    }
}

int get_data(pnode nd){
    if(nd==NULL){
        return -1;
    }
    else{
        return nd->data;
    }
}

pnode get_list(const phead listh){
    if(listh==NULL){
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
