#include "error.h"
#include "hash.h"
#include "struct_list.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct hash_info {
    phead* bins;                //array of lists
    int size;                   //hash_table size
    int (*h)(void *);
    int type;
}hash_info;

phnode create_phnode(void* el);   //creates a hash_node

phash create_hashtable(int hash_table_size ,int(*h)(void *),int type){
    if((type != 1 && type!=0)||hash_table_size<=0){
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
    tmp->bins=malloc(sizeof(phead)*hash_table_size);
    if(tmp->bins==NULL){
        error_val=HASH_BINS_CR_MALLOC;
        free (tmp);
        return NULL;
    }
    tmp->type=type;
    for(i=0;i<hash_table_size;i++){
        tmp->bins[i]=cr_list();
    }
    error_val=OK_SUCCESS;
    return tmp;
}

rcode h_insert(phash a,uint32_t data,uint32_t prev){
    int pos=a->h((void*) &data);
    rcode stat;
    if(a->type==0)
        stat=insert(a->bins[pos],data,prev);
    else if(a->type==1){
        stat=insert_sorted(a->bins[pos],data,prev);
    }
    error_val=stat;
    return stat;
}

int in_hash(phash a,uint32_t data){
    if(a==NULL){
        error_val=NULL_HASH;
        return -1;
    }
    int pos=a->h((void*) &data),stat;
    if(a->type==0)
        stat= in(a->bins[pos],data);
    else if(a->type==1)
        stat= ins(a->bins[pos],data);
    error_val=stat;
    return stat;
}

int h_delete(phash a,uint32_t data){
    if(a==NULL){
        error_val=NULL_HASH;
        return -1;
    }
    int pos=a->h((void*)&data);
    delete(a->bins[pos],data);
    error_val=OK_SUCCESS;
    return OK_SUCCESS;
}

rcode ds_hash(phash a){
    if(a==NULL){
        error_val=NULL_HASH;
        return NULL_HASH;
    }
    int i;
    for(i=0;i<a->size;i++){
        ds_list(a->bins[i]);
    }
    free(a->bins);
    free(a);
    return OK_SUCCESS;
}

int ret_prev(phash a,uint32_t data){
    if(a==NULL){
        error_val=NULL_HASH;
        return -1;
    }
    int pos=a->h((void*) &data),stat;
    stat=get_prev(a->bins[pos],data);
    error_val=stat;
    return stat;
}
