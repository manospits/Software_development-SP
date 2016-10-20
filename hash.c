#include "hash.h"
#include "intlist.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct hash_info {
    phead* bins;                //array of lists
    int size;                   //hash_table size
    int (*h)(void *);
    int type;
}hash_info;

phash create_hashtable(int hash_table_size ,int(*h)(void *),int type){
    phash tmp;
    int i;
    tmp=malloc(sizeof(struct hash_info));
    if(tmp==NULL){
        fprintf(stderr,"Error allocating memory -hash.c\n");
        exit(1);
    }
    tmp->size=hash_table_size;
    tmp->h=h;
    tmp->bins=malloc(sizeof(phead)*hash_table_size);
    if(type != 1 && type!=0){
         fprintf(stderr,"create_hashtable: type must be 1 or 0.\n");
    }
    tmp->type=type;
    if(tmp->bins==NULL){
        fprintf(stderr,"Error allocating memory -hash.c\n");
        exit(1);
    }
    for(i=0;i<hash_table_size;i++){
        tmp->bins[i]=cr_list();
    }
    return tmp;
}

int h_insert(phash a,int data){
    int pos=a->h((void*) &data);
    if(a->type==0)
        insert(a->bins[pos],data);
    else if(a->type==1){
         insert_sorted(a->bins[pos],data);
    }
    return 0;
}

int in_hash(phash a,int data){
    int pos=a->h((void*) &data),stat;
    if(a->type==0)
        stat= in(a->bins[pos],data);
    else if(a->type==1)
        stat= ins(a->bins[pos],data);
    return stat;
}

int h_delete(phash a,int data){
    int pos=a->h((void*)&data);
    delete(a->bins[pos],data);
    return 0;
}

void ds_hash(phash a){
    int i;
    for(i=0;i<a->size;i++){
        ds_list(a->bins[i]);
    }
    free(a->bins);
    free(a);
}

