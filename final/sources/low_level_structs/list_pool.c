#include <stdlib.h>
#include "../include/list_pool.h"
#include "../include/struct_list.h"
#include "../include/error.h"

typedef struct list_pool{
    stphead* lists;
    int nextavailable;
    int size;
}list_pool;


lpool create_listpool(){
    lpool l;
    int i;
    if((l=malloc(sizeof(list_pool)))==NULL){
        print_errorv(LP_MALLOC_FAIL);
        /*error_val=LP_MALLOC_FAIL;*/
        return NULL;
    }
    l->nextavailable=0;
    l->size=LPOOL_INIT_SIZE;
    if((l->lists=malloc(sizeof(phead)*LPOOL_INIT_SIZE))==NULL){
        print_errorv(LP_MALLOC_FAIL);
        /*error_val=LP_MALLOC_FAIL;*/
        return NULL;
    }
    for(i=0;i<LPOOL_INIT_SIZE;i++){
        l->lists[i]=st_cr_list();
    }
    return l;
}

stphead get_alist(lpool l){
    if(l->nextavailable==l->size){
        if((l->lists=realloc(l->lists,l->size*2*sizeof(phead)))==NULL){
            print_errorv(LP_REALLOC_FAIL);
            /*error_val=LP_REALLOC_FAIL;*/
            return NULL;
        }
        int i;
        for(i=l->size;i<l->size*2;i++){
            l->lists[i]=st_cr_list();
        }
        l->size*=2;
    }
    st_empty_list(l->lists[l->nextavailable]);
    return l->lists[l->nextavailable++];
}

rcode empty_lists(lpool l){
    /*int i;*/
    /*for(i=0;i<l->nextavailable;i++){*/
        /*st_empty_list(l->lists[i]);*/
    /*}*/
    l->nextavailable=0;
    /*error_val=OK_SUCCESS;*/
    return OK_SUCCESS;
}

rcode ds_pool(lpool l){
    int i;
    for(i=0;i<l->size;i++){
        st_ds_list(l->lists[i]);
    }
    free(l->lists);
    free(l);
    /*error_val=OK_SUCCESS;*/
    return error_val;
}
