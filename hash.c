#include "error.h"
#include "hash.h"
#include "struct_list.h"
#include "intlist.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct hash_info {
    stphead* bins;                //array of lists
    int size;                   //hash_table size
    int (*h)(void *,void*);
    int type;
    phead accessed_bins;
}hash_info;

phnode create_phnode(void* el);   //creates a hash_node

phash create_hashtable(int hash_table_size ,int(*h)(void *,void*),int type){
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
    tmp->bins=malloc(sizeof(stphead)*hash_table_size);
    if(tmp->bins==NULL){
        error_val=HASH_BINS_CR_MALLOC;
        free (tmp);
        return NULL;
    }
    tmp->type=type;
    for(i=0;i<hash_table_size;i++){
        tmp->bins[i]=st_cr_list();
    }
    tmp->accessed_bins=cr_list();
    error_val=OK_SUCCESS;
    return tmp;
}

rcode h_insert(phash a,uint32_t data,uint32_t tag){
    int pos=a->h((void*) &data,(void *)&(a->size));
    if(st_get_size(a->bins[pos])==0){
        insert(a->accessed_bins,pos);
    }
    rcode stat;
    if(a->type==0)
        stat=st_insert(a->bins[pos],data,tag);
    else if(a->type==1){
        stat=st_insert_sorted(a->bins[pos],data,tag);
    }
    error_val=stat;
    return stat;
}

int in_hash(phash a,uint32_t data){
    if(a==NULL){
        error_val=NULL_HASH;
        return -1;
    }
    int pos=a->h((void*) &data,(void *)&(a->size)),stat;
    if(a->type==0)
        stat= st_in(a->bins[pos],data);
    else if(a->type==1)
        stat= st_ins(a->bins[pos],data);
    error_val=stat;
    return stat;
}

int h_delete(phash a,uint32_t data){
    if(a==NULL){
        error_val=NULL_HASH;
        return -1;
    }
    int pos=a->h((void*)&data,(void *)&(a->size));
    st_delete(a->bins[pos],data);
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
        st_ds_list(a->bins[i]);
    }
    ds_list(a->accessed_bins);
    free(a->bins);
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
        st_empty_list(a->bins[i]);
        pop_front(a->accessed_bins);
    }
    return OK_SUCCESS;
}

int ret_tag(phash a,uint32_t data){
    if(a==NULL){
        error_val=NULL_HASH;
        return -1;
    }
    int pos=a->h((void*) &data,(void *)&(a->size)),stat;
    stat=st_get_tag(a->bins[pos],data);
    error_val=stat;
    return stat;
}
