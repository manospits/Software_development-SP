#include "error.h"
#include "hash.h"
#include "struct_list.h"
#include "intlist.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct bucket {
    char *bucketpos;
    int size;
    int elements;
    int last_pos;
}bucket ;

typedef struct hash_info {
    phead bins[HASHTABLE_SIZE];                //array of lists
    int size;                   //hash_table size
    int (*h)(void *,void*);
    int type;
    phead accessed_bins;
}hash_info;

phnode create_phnode(void* el);   //creates a hash_node

phash create_hashtable(int hash_table_size ,int(*h)(void *,void*)){
    if(hash_table_size<=0){
        error_val=HASH_WRONG_PARMS;
        return NULL;
    }
    phash tmp;
    int i;
    tmp=malloc(sizeof(struct hash_info));
    if(tmp==NULL){
        error_val=HASH_CR_MALLOC;
        return NULL;
    }
    tmp->size=hash_table_size;
    tmp->h=h;
    for(i=0;i<hash_table_size;i++){
        tmp->bins[i]=NULL;
    }
    tmp->accessed_bins=cr_list();
    error_val=OK_SUCCESS;
    return tmp;
}

rcode h_insert(phash a,uint32_t data){
    int pos=a->h((void*) &data,(void *)&(a->size));
    if(a->bins[pos]==NULL){
        a->bins[pos]=cr_list();
    }
    if(get_size(a->bins[pos])==0){
        insert_back(a->accessed_bins,pos);
    }
    rcode stat;
    stat=insert_back(a->bins[pos],data);
    error_val=stat;
    return stat;
}

int in_hash(phash a,uint32_t data){
    if(a==NULL){
        error_val=NULL_HASH;
        return -1;
    }
    int pos=a->h((void*) &data,(void *)&(a->size)),stat;
    if(a->bins[pos]==NULL){
        return 0;
    }
    if(a->type==0)
        stat= in(a->bins[pos],data);
    error_val=stat;
    return stat;
}


rcode ds_hash(phash a){
    if(a==NULL){
        error_val=NULL_HASH;
        return NULL_HASH;
    }
    int i;
    for(i=0;i<a->size;i++){
        if(a->bins[i]!=NULL){
           ds_list(a->bins[i]);
        }
    }
    ds_list(a->accessed_bins);
    free(a);
    return OK_SUCCESS;
}

rcode empty_hash(phash a){
    if(a==NULL){
        error_val=NULL_HASH;
        return NULL_HASH;
    }
    int i;
    while(get_size(a->accessed_bins)!=0){
        i=peek(a->accessed_bins);
        empty_list(a->bins[i]);
        a->bins[i]=NULL;
        pop_front(a->accessed_bins);
    }
    return OK_SUCCESS;
}


