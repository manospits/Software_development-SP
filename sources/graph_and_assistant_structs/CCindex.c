#include "../include/CCindex.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../include/intlist.h"
#include "../include/struct_list.h"
#include "../include/error.h"
#include "../include/graph.h"
#include "../include/buffer.h"
#include "../include/list_pool.h"
#include "../include/index.h"

typedef struct UpdateIndex{
    stphead *uindex;
    int size;
}UpdateIndex;

typedef struct ccindex_record{
    uint32_t cc;
    uint32_t version;
}ccindex_record;

typedef struct cc_extras{
    int updated;
    int vals;
}cc_extras;

typedef struct CC{
    ccindex_record *ccindex; //CCindex
    UpdateIndex UpdateIndex;
    lpool lists;
    phead idlist;
    int *updated; //
    int *vals; //
    int version;
    int index_size;
    int updated_size;
    int next_component_num;
    int update_queries;
    int total_update_queries;
    int total_queries;
    int queries;
    uint32_t metricVal;
} CC;

typedef struct findret{
    stphead a;
    stpnode b;
}findret;

int unify(CC_index c,uint32_t cca,uint32_t ccb);
int unify_t(CC_index c,uint32_t cca,uint32_t ccb,uint32_t version);
int same_component_edge(CC_index c, uint32_t  nodeida,uint32_t nodeidb);

CC_index CC_create_index(pGraph g){
    Index_ptr inIndex=ret_inIndex(g);
    Index_ptr outIndex=ret_outIndex(g);
    CC_index tmp;
    int i,index,max_nodes=ret_biggest_node(inIndex)+1,return_value,l,state=0;
    if((tmp=malloc(sizeof(struct CC)))==NULL){
        print_errorv(CC_MALLOC_FAIL);
        /*error_val=CC_MALLOC_FAIL;*/
        return NULL;
    }
    if((tmp->ccindex=malloc(sizeof(ccindex_record)*max_nodes))==NULL){
        free(tmp);
        print_errorv(CC_MALLOC_FAIL);
        /*error_val=CC_MALLOC_FAIL;*/
        return NULL;
    }
    for(i=0;i<max_nodes;i++){
        tmp->ccindex[i].cc=-1;
        tmp->ccindex[i].version=0;
    }
    tmp->idlist=cr_list();
    tmp->version=0;
    tmp->update_queries=0;
    tmp->total_update_queries=0;
    tmp->total_queries=0;
    tmp->queries=0;
    tmp->index_size = max_nodes;
    tmp->next_component_num = 0;
    tmp->lists=create_listpool();
    //Creating CC INDEX
    graphNode temp_node;
    pBuffer temp_buffer;
    ptr buffer_ptr_to_listnode;
    plnode listnode;
    Index_ptr Ind[2]={outIndex,inIndex};
    for(l=0;l<max_nodes;l++){
        if(tmp->ccindex[l].cc==-1){
            if ((return_value = insert_back(tmp->idlist,l)) != OK_SUCCESS)
            {
                print_errorv(CC_CREATION_FAIL);
                ds_list(tmp->idlist);
                return NULL;
            }
            while(get_size(tmp->idlist) > 0)
            {
                if ((temp_node = peek_back(tmp->idlist)) < 0)
                {
                    print_errorv(CC_CREATION_FAIL);
                    ds_list(tmp->idlist);
                    return NULL;
                }
                if ((return_value = pop_back(tmp->idlist)) < 0)
                {
                    print_errorv(CC_CREATION_FAIL);
                    ds_list(tmp->idlist);
                    return NULL;
                }
                for(index=0;index<2;index++){
                    buffer_ptr_to_listnode = getListHead(Ind[index], temp_node);
                    if (buffer_ptr_to_listnode == -1)
                    {   // node has no neighbors
                        continue;
                    }
                    else if (buffer_ptr_to_listnode < 0)
                    {   // an error occurred
                        print_errorv(CC_CREATION_FAIL);
                        ds_list(tmp->idlist);
                        return NULL;
                    }
                    if ((temp_buffer = return_buffer(Ind[index])) == NULL)
                    {
                        print_errorv(CC_CREATION_FAIL);
                        ds_list(tmp->idlist);
                        return NULL;
                    }
                    if ((listnode = getListNode(temp_buffer, buffer_ptr_to_listnode)) == NULL)
                    {
                        print_errorv(CC_CREATION_FAIL);
                        ds_list(tmp->idlist);
                        return NULL;
                    }
                    i = 0;
                    while (listnode->neighbor[i] != -1)
                    {
                        if (tmp->ccindex[listnode->neighbor[i]].cc==-1)
                        {
                            state=1;
                            tmp->ccindex[listnode->neighbor[i]].cc=tmp->next_component_num;
                            if (insert_back(tmp->idlist, listnode->neighbor[i]) != OK_SUCCESS)
                            {
                                print_errorv(CC_CREATION_FAIL);
                                ds_list(tmp->idlist);
                                return NULL;
                            }
                        }
                        i++;
                        if (i == N)
                        {
                            if (listnode->nextListNode == -1)   // if there are no more neighbors, break
                                break;
                            if ((listnode = getListNode(temp_buffer, listnode->nextListNode)) == NULL)
                            {
                                print_errorv(CC_CREATION_FAIL);
                                ds_list(tmp->idlist);
                                return NULL;
                            }
                            i = 0;
                        }
                    }
                }
            }
            if(state==1){
                tmp->next_component_num++;
                state=0;
            }
        }
    }
    printf("Number of components before Workload : %d \n",tmp->next_component_num);
    tmp->updated_size=tmp->next_component_num;
    if((tmp->updated=malloc((tmp->updated_size)*sizeof(int)))==NULL){
        free(tmp->ccindex);
        free(tmp);
        print_errorv(CC_MALLOC_FAIL);
        /*error_val=CC_MALLOC_FAIL;*/
        return NULL;
    }
    if((tmp->vals=malloc((tmp->updated_size)*sizeof(int)))==NULL){
        free(tmp->ccindex);
        free(tmp);
        print_errorv(CC_MALLOC_FAIL);
        /*error_val=CC_MALLOC_FAIL;*/
        return NULL;
    }
    for(i=0;i<tmp->updated_size;i++){
        tmp->updated[i]=-1;
    }
    if((tmp->UpdateIndex.uindex=malloc(tmp->updated_size*sizeof(stphead)))==NULL){
        free(tmp->ccindex);
        free(tmp->updated);
        free(tmp);
        print_errorv(CC_MALLOC_FAIL);
        /*error_val=CC_MALLOC_FAIL;*/
        return NULL;
    }
    tmp->UpdateIndex.size = tmp->updated_size;
    return tmp;
}

rcode CC_destroy(CC_index c){
    ds_list(c->idlist);
    ds_pool(c->lists);
    free(c->ccindex);
    free(c->updated);
    free(c->vals);
    free(c->UpdateIndex.uindex);
    free(c);
    /*error_val=OK_SUCCESS;*/
    return OK_SUCCESS;
}

rcode CC_insertNewEdge_t(CC_index c,uint32_t nodeida,uint32_t nodeidb,uint32_t version){
    uint32_t max=nodeidb;
    if(nodeida==nodeidb){
        return OK_SUCCESS;
    }
    (nodeida>nodeidb) ? (max=nodeida) : (max=nodeidb);
    if(max>=c->index_size){
        int prev_size=c->index_size;
        int next_size=c->index_size,i;
        while(next_size<=max){
            next_size<<=1;
        }
        c->index_size=next_size;
        if((c->ccindex=realloc(c->ccindex,next_size*sizeof(ccindex_record)))==NULL){
            print_errorv(CC_REALLOC_FAIL);
            /*error_val=CC_REALLOC_FAIL;*/
            return CC_REALLOC_FAIL;
        }
        for(i=prev_size;i<next_size;i++){
            c->ccindex[i].cc=-1;
            c->ccindex[i].version=0;
        }
    }
    if(c->ccindex[nodeida].cc==-1 && c->ccindex[nodeidb].cc==-1 ){
        c->ccindex[nodeida].cc=c->next_component_num;
        c->ccindex[nodeida].version=version;
        c->ccindex[nodeidb].cc=c->next_component_num;
        c->ccindex[nodeidb].version=version;
        c->next_component_num++;
        if(c->updated_size<=c->next_component_num){
            int i,next_size=c->updated_size;
            while(next_size<=c->next_component_num){
                next_size*=2;
            }
            c->updated=realloc(c->updated,next_size*(sizeof(int)));
            c->vals=realloc(c->vals,next_size*(sizeof(int)));
            c->UpdateIndex.uindex=realloc(c->UpdateIndex.uindex,next_size*sizeof(stphead));
            for(i=c->updated_size;i<next_size;i++){
                c->updated[i]=-1;
            }
            c->updated_size=next_size;
        }
    }
    else if(c->ccindex[nodeida].cc==-1||c->ccindex[nodeidb].cc==-1){
        if(c->ccindex[nodeida].cc==-1){
            c->ccindex[nodeida].cc=c->ccindex[nodeidb].cc;
            c->ccindex[nodeida].version=version;
        }
        if(c->ccindex[nodeidb].cc==-1){
            c->ccindex[nodeidb].cc=c->ccindex[nodeida].cc;
            c->ccindex[nodeidb].version=version;
        }
    }
    else if(c->ccindex[nodeida].cc!=c->ccindex[nodeidb].cc){
        if(c->updated[c->ccindex[nodeida].cc]<c->version){
            c->UpdateIndex.uindex[c->ccindex[nodeida].cc]=get_alist(c->lists);
            st_insert_back2(c->UpdateIndex.uindex[c->ccindex[nodeida].cc],c->ccindex[nodeida].cc,0,version);
        }
        if(c->updated[c->ccindex[nodeidb].cc]<c->version){
            c->UpdateIndex.uindex[c->ccindex[nodeidb].cc]=get_alist(c->lists);
            st_insert_back2(c->UpdateIndex.uindex[c->ccindex[nodeidb].cc],c->ccindex[nodeidb].cc,0,version);
        }
        unify_t(c,c->ccindex[nodeida].cc,c->ccindex[nodeidb].cc,0);
        if(c->updated[c->ccindex[nodeida].cc]<c->version){
            c->updated[c->ccindex[nodeida].cc]=c->version;
        }
        if(c->updated[c->ccindex[nodeidb].cc]<c->version){
            c->updated[c->ccindex[nodeidb].cc]=c->version;
        }
    }
    /*error_val=OK_SUCCESS;*/
    return OK_SUCCESS;
}

findret find_last(CC_index c,uint32_t cc){
    stpnode tmpnode;
    findret retval;
    uint32_t ccnum;
    tmpnode=st_peek_back_node(c->UpdateIndex.uindex[cc]);
    ccnum=cc;
    while(ccnum!=tmpnode->data){
        insert_back(c->idlist,ccnum);
        ccnum=tmpnode->data;
        tmpnode=st_peek_back_node(c->UpdateIndex.uindex[tmpnode->data]);
    }
    retval.a=c->UpdateIndex.uindex[cc];
    retval.b=tmpnode;
    while(get_size(c->idlist) > 0)
    {
        ccnum=peek_back(c->idlist);
        pop_back(c->idlist);
        tmpnode=st_peek_back_node(c->UpdateIndex.uindex[tmpnode->data]);
        tmpnode->data=retval.b->data;
    }
    return retval;
}

findret find_last_nc(CC_index c,uint32_t cc){
    stpnode tmpnode;
    findret retval;
    uint32_t ccnum;
    tmpnode=st_peek_back_node(c->UpdateIndex.uindex[cc]);
    ccnum=cc;
    while(ccnum!=tmpnode->data){
        ccnum=tmpnode->data;
        tmpnode=st_peek_back_node(c->UpdateIndex.uindex[tmpnode->data]);
    }
    retval.a=c->UpdateIndex.uindex[cc];
    retval.b=tmpnode;
    return retval;
}

findret find(CC_index c,uint32_t cc,uint32_t version){
    stpnode tmpnode,startingnode=NULL;
    findret retval;
    uint32_t ccnum;
    iterator it,it2;
    it=st_ret_iterator(c->UpdateIndex.uindex[cc]);
    while(it!=-1){
        tmpnode=st_get_iterator_data(c->UpdateIndex.uindex[cc],it);
        if(tmpnode->tag<=version){
            startingnode=tmpnode;
        }
        else{
            break;
        }
        it=st_advance_iterator(c->UpdateIndex.uindex[cc],it);
    }
    tmpnode=startingnode;
    ccnum=cc;
    /*if(ccnum!=tmpnode->data){*/
        /*it2=st_ret_iterator(c->UpdateIndex.uindex[tmpnode->data]);*/
        /*ccnum=tmpnode->data;*/
        /*while(it2!=-1){*/
            /*tmpnode=st_get_iterator_data(c->UpdateIndex.uindex[tmpnode->data],it2);*/
            /*if(tmpnode->tag<=version){*/
                /*startingnode=tmpnode;*/
            /*}*/
            /*else{*/
                /*break;*/
            /*}*/
            /*it2=st_advance_iterator(c->UpdateIndex.uindex[tmpnode->data],it2);*/
        /*}*/
        /*retval=find(c,startingnode->data,version);*/
    /*}*/
    /*else{*/
        /*retval.a=c->UpdateIndex.uindex[cc];*/
        /*retval.b=checkingnode;*/
    /*}*/
    if(startingnode==NULL){
        retval.a=NULL;
        retval.b=NULL;
        return retval;
    }
    while(ccnum!=tmpnode->data){
        it2=st_ret_iterator(c->UpdateIndex.uindex[tmpnode->data]);
        ccnum=tmpnode->data;
        while(it2!=-1){
            tmpnode=st_get_iterator_data(c->UpdateIndex.uindex[tmpnode->data],it2);
            if(tmpnode->tag<=version){
                startingnode=tmpnode;
            }
            else{
                break;
            }
            it2=st_advance_iterator(c->UpdateIndex.uindex[tmpnode->data],it2);
        }
        tmpnode=startingnode;
    }
    retval.a=c->UpdateIndex.uindex[tmpnode->data];
    retval.b=tmpnode;
    return retval;
}

int unify(CC_index c,uint32_t cca,uint32_t ccb){
    findret ccarootinfo=find_last(c,cca);
    findret ccbrootinfo=find_last(c,ccb);
    stpnode xroot=ccarootinfo.b;
    stpnode yroot=ccbrootinfo.b;
    if(xroot->data==yroot->data){
        return OK_SUCCESS;
    }
    if (xroot->data2<yroot->data2){
        xroot->data=yroot->data;
    }
    else if(xroot->data2>yroot->data2){
        yroot->data=xroot->data;
    }
    else{
        yroot->data=xroot->data;
        xroot->data2+=1;
    }
    return OK_SUCCESS;
}

int unify_t(CC_index c,uint32_t cca,uint32_t ccb,uint32_t version){
    findret ccarootinfo=find_last_nc(c,cca);
    findret ccbrootinfo=find_last_nc(c,ccb);
    stpnode xroot=ccarootinfo.b;
    stpnode yroot=ccbrootinfo.b;
    if(xroot->data==yroot->data){
        return OK_SUCCESS;
    }
    if (xroot->data2<yroot->data2){
        if(xroot->tag<version){
            st_insert_back2(ccarootinfo.a,yroot->data,xroot->data2,version);
        }
        else{
            xroot->data=yroot->data;
        }
    }
    else if(xroot->data2>yroot->data2){
        if(yroot->tag<version){
            st_insert_back2(ccbrootinfo.a,xroot->data,yroot->data2,version);
        }
        else{
            yroot->data=xroot->data;
        }
    }
    else{
        if(yroot->tag<version){
            st_insert_back2(ccbrootinfo.a,xroot->data,yroot->data2,version);
        }
        else{
            yroot->data=xroot->data;
        }
        if(xroot->tag<version){
            st_insert_back2(ccarootinfo.a,xroot->data,xroot->data2+1,version);
        }
        else{
            xroot->data2+=1;
        }
    }
    return OK_SUCCESS;
}

rcode CC_insertNewEdge(CC_index c,uint32_t nodeida,uint32_t nodeidb){
    if(nodeida==nodeidb){
        return OK_SUCCESS;
    }
    uint32_t max=nodeidb;
    (nodeida>nodeidb) ? (max=nodeida) : (max=nodeidb);
    if(max>=c->index_size){
        int prev_size=c->index_size;
        int next_size=c->index_size,i;
        while(next_size<=max){
            next_size<<=1;
        }
        c->index_size=next_size;
        if((c->ccindex=realloc(c->ccindex,next_size*sizeof(ccindex_record)))==NULL){
            print_errorv(CC_REALLOC_FAIL);
            /*error_val=CC_REALLOC_FAIL;*/
            return CC_REALLOC_FAIL;
        }
        for(i=prev_size;i<next_size;i++){
            c->ccindex[i].cc=-1;
            c->ccindex[i].version=0;
        }
    }
    if(c->ccindex[nodeida].cc==-1 && c->ccindex[nodeidb].cc==-1 ){
        c->ccindex[nodeida].cc=c->next_component_num;
        c->ccindex[nodeidb].cc=c->next_component_num;
        c->next_component_num++;
        if(c->updated_size<=c->next_component_num){
            int i,next_size=c->updated_size;
            while(next_size<=c->next_component_num){
                next_size*=2;
            }
            c->updated=realloc(c->updated,next_size*(sizeof(int)));
            c->vals=realloc(c->vals,next_size*(sizeof(int)));
            c->UpdateIndex.uindex=realloc(c->UpdateIndex.uindex,next_size*sizeof(phead));
            for(i=c->updated_size;i<next_size;i++){
                c->updated[i]=-1;
            }
            c->updated_size=next_size;
        }
    }
    else if(c->ccindex[nodeida].cc==-1||c->ccindex[nodeidb].cc==-1){
        if(c->ccindex[nodeida].cc==-1){
            c->ccindex[nodeida].cc=c->ccindex[nodeidb].cc;
        }
        if(c->ccindex[nodeidb].cc==-1){
            c->ccindex[nodeidb].cc=c->ccindex[nodeida].cc;
        }
    }
    else if(c->ccindex[nodeida].cc!=c->ccindex[nodeidb].cc){
        if(c->updated[c->ccindex[nodeida].cc]<c->version){
            c->UpdateIndex.uindex[c->ccindex[nodeida].cc]=get_alist(c->lists);
            st_insert_back2(c->UpdateIndex.uindex[c->ccindex[nodeida].cc],c->ccindex[nodeida].cc,0,0);
        }
        if(c->updated[c->ccindex[nodeidb].cc]<c->version){
            c->UpdateIndex.uindex[c->ccindex[nodeidb].cc]=get_alist(c->lists);
            st_insert_back2(c->UpdateIndex.uindex[c->ccindex[nodeidb].cc],c->ccindex[nodeidb].cc,0,0);
        }
        unify(c,c->ccindex[nodeida].cc,c->ccindex[nodeidb].cc);
        if(c->updated[c->ccindex[nodeida].cc]<c->version){
            c->updated[c->ccindex[nodeida].cc]=c->version;
        }
        if(c->updated[c->ccindex[nodeidb].cc]<c->version){
            c->updated[c->ccindex[nodeidb].cc]=c->version;
        }
    }
    /*error_val=OK_SUCCESS;*/
    return OK_SUCCESS;
}

int CC_same_component_2(CC_index c,uint32_t nodeida ,uint32_t nodeidb){
    int m,a,b,ucca,uccb;
    c->queries++;
    if((m=(c->ccindex[nodeidb].cc==c->ccindex[nodeida].cc))){
        return 1;
    }
    else{
        c->update_queries++;
        uint32_t ca=c->ccindex[nodeida].cc;
        uint32_t cb=c->ccindex[nodeidb].cc;
        a=(c->updated[ca]<c->version);
        b=(c->updated[cb]<c->version);
        if(a||b){
            if(a&&b){
                return m;
            }
            else if(a && !b){
                findret uccbi=find_last(c,c->ccindex[nodeidb].cc);
                ucca=c->ccindex[nodeida].cc;
                if(uccbi.a == NULL){
                    uccb=c->ccindex[nodeidb].cc;
                }
                else{
                    uccb=uccbi.b->data;
                }
                    return (ucca==uccb);
            }
            else{
                findret uccai=find_last(c,c->ccindex[nodeida].cc);
                if(uccai.a == NULL){
                    ucca=c->ccindex[nodeida].cc;
                }
                else{
                    ucca=uccai.b->data;
                }
                uccb=c->ccindex[nodeidb].cc;
                return (ucca==uccb);
            }
        }
        else if(!a && !b){
            findret uccai=find_last(c,c->ccindex[nodeida].cc);
            findret uccbi=find_last(c,c->ccindex[nodeidb].cc);
            if(uccai.a == NULL){
                ucca=c->ccindex[nodeida].cc;
            }
            else{
                ucca=uccai.b->data;
            }
            if(uccbi.a == NULL){
                uccb=c->ccindex[nodeidb].cc;
            }
            else{
                uccb=uccbi.b->data;
            }
            return (ucca==uccb);
        }
    }
    return -1;
}

int CC_same_component_2_t(CC_index c,uint32_t nodeida ,uint32_t nodeidb,uint32_t version,int *queries, int *update_queries){
    int m,a,b,ucca,uccb;
    (*queries)++;
    if((m=((c->ccindex[nodeidb].cc==c->ccindex[nodeida].cc) && (c->ccindex[nodeida].version <= version) && (c->ccindex[nodeidb].version <=version)))){
        return 1;
    }
    else{
        (*update_queries)++;
        uint32_t ca=c->ccindex[nodeida].cc;
        uint32_t cb=c->ccindex[nodeidb].cc;
        a=(c->updated[ca]<c->version);
        b=(c->updated[cb]<c->version);
        if(a||b){
            if(a&&b){
                return m;
            }
            else if(a && !b){
                findret uccbi=find(c,c->ccindex[nodeidb].cc,version);
                ucca=c->ccindex[nodeida].cc;
                if(uccbi.a == NULL){
                    uccb=c->ccindex[nodeidb].cc;
                }
                else{
                    uccb=uccbi.b->data;
                }
                    return (ucca==uccb);
            }
            else{
                findret uccai=find(c,c->ccindex[nodeida].cc,version);
                if(uccai.a == NULL){
                    ucca=c->ccindex[nodeida].cc;
                }
                else{
                    ucca=uccai.b->data;
                }
                uccb=c->ccindex[nodeidb].cc;
                return (ucca==uccb);
            }
        }
        else if(!a && !b){
            findret uccai=find(c,c->ccindex[nodeida].cc,version);
            findret uccbi=find(c,c->ccindex[nodeidb].cc,version);
            if(uccai.a == NULL){
                ucca=c->ccindex[nodeida].cc;
            }
            else{
                ucca=uccai.b->data;
            }
            if(uccbi.a == NULL){
                uccb=c->ccindex[nodeidb].cc;
            }
            else{
                uccb=uccbi.b->data;
            }
            return (ucca==uccb);
        }
    }
    return -1;
}

rcode check_rebuild(CC_index c){
    if((double)c->update_queries/(double)c->queries>METRIC_VAL){
        CC_rebuildIndexes(c);
        c->total_queries+=c->queries;
        c->queries=0;
        c->total_update_queries+=c->update_queries;
        c->update_queries=0;
    }
    return OK_SUCCESS;
}

rcode check_rebuild_t(CC_index c,int *queries,int *update_queries){
    if((double)*update_queries/(double)*queries>METRIC_VAL){
        CC_rebuildIndexes(c);
        c->total_queries+=*queries;
        *queries=0;
        c->total_update_queries+=*update_queries;
        *update_queries=0;
    }
    return OK_SUCCESS;
}

rcode CC_rebuildIndexes(CC_index c){
    int j;
    for(j=0;j<c->updated_size;j++){
        if(c->updated[j]==c->version){
            findret rootinfo=find_last(c,j);
            c->vals[j]=rootinfo.b->data;
        }
    }
    for(j=0;j<c->index_size;j++){
        if(c->ccindex[j].cc!=-1 && c->updated[c->ccindex[j].cc]==c->version ){
            c->ccindex[j].cc=c->vals[c->ccindex[j].cc];
        }
    }
    c->version++;
    empty_lists(c->lists);
    return OK_SUCCESS;
}

void print_max(CC_index c ){
    int max,i;
    max=0;
    for(i=0;i<c->index_size;i++){
        if(c->ccindex[i].cc>max){
            /*printf("%d\n",c->ccindex[i]);*/
            max=c->ccindex[i].cc;
        }
    }
    printf("%d\n",max+1);
}
