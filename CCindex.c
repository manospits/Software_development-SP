#include "CCindex.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "intlist.h"
#include "error.h"
#include "graph.h"
#include "buffer.h"
#include "list_pool.h"
#include "index.h"

typedef struct UpdateIndex{
    phead *uindex;
    int size;
}UpdateIndex;

typedef struct CC{
    uint32_t *ccindex; //CCindex
    int *updated; //
    int *marked; //
    int *vals; //
    UpdateIndex UpdateIndex;
    uint32_t metricVal;
    int index_size;
    int updated_size;
    int next_component_num;
    int update_queries;
    int queries;
    int metric;
    long int version;
    phead idlist;
    phead uidlist;
    lpool lists;
    long int check;
} CC;

int same_component_edge(CC_index c, uint32_t  nodeida,uint32_t nodeidb);

CC_index CC_create_index(pGraph g){
    Index_ptr inIndex=ret_inIndex(g);
    Index_ptr outIndex=ret_outIndex(g);
    CC_index tmp;
    int i,index,max_nodes=ret_biggest_node(inIndex)+1,return_value,l;
    if((tmp=malloc(sizeof(struct CC)))==NULL){
        error_val=CC_MALLOC_FAIL;
        return NULL;
    }
    if((tmp->ccindex=malloc(sizeof(uint32_t)*max_nodes))==NULL){
        free(tmp);
        error_val=CC_MALLOC_FAIL;
        return NULL;
    }
    for(i=0;i<max_nodes;i++){
        tmp->ccindex[i]=-1;
    }
    tmp->idlist=cr_list();
    tmp->uidlist=cr_list();
    tmp->version=0;
    tmp->check=0;
    tmp->update_queries=0;
    tmp->queries=0;
    tmp->metric= METRIC_INIT_VALUE;
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
        if(tmp->ccindex[l]==-1){
            if ((return_value = insert_back(tmp->idlist,l)) != OK_SUCCESS)
            {
                ds_list(tmp->idlist);
                error_val = return_value;
                return NULL;
            }
            while(get_size(tmp->idlist) > 0)
            {
                if ((temp_node = peek(tmp->idlist)) < 0)
                {
                    ds_list(tmp->idlist);
                    error_val = temp_node;
                    return NULL;
                }
                if ((return_value = pop_front(tmp->idlist)) < 0)
                {
                    ds_list(tmp->idlist);
                    error_val = return_value;
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
                        ds_list(tmp->idlist);
                        error_val = return_value;
                        return NULL;
                    }
                    if ((temp_buffer = return_buffer(Ind[index])) == NULL)
                    {
                        return_value = error_val;
                        ds_list(tmp->idlist);
                        error_val = return_value;
                        return NULL;
                    }
                    if ((listnode = getListNode(temp_buffer, buffer_ptr_to_listnode)) == NULL)
                    {
                        return_value = error_val;
                        ds_list(tmp->idlist);
                        error_val = return_value;
                        return NULL;
                    }
                    i = 0;
                    while (listnode->neighbor[i] != -1)
                    {
                        if (tmp->ccindex[listnode->neighbor[i]]==-1)
                        {    // if this node hasn't been visited yet
                            tmp->ccindex[listnode->neighbor[i]]=tmp->next_component_num;
                            if ((return_value =insert_back(tmp->idlist, listnode->neighbor[i])) != OK_SUCCESS)
                            {
                                ds_list(tmp->idlist);
                                error_val = return_value;
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
                                return_value = error_val;
                                ds_list(tmp->idlist);
                                error_val = return_value;
                                return NULL;
                            }
                            i = 0;
                        }
                    }
                }
            }
            tmp->next_component_num++;
        }
    }
    tmp->updated_size=tmp->next_component_num-1;
    if((tmp->updated=malloc((tmp->updated_size)*sizeof(int)))==NULL){
        free(tmp->ccindex);
        free(tmp);
        error_val=CC_MALLOC_FAIL;
        return NULL;
    }
    if((tmp->vals=malloc((tmp->updated_size)*sizeof(int)))==NULL){
        free(tmp->ccindex);
        free(tmp);
        error_val=CC_MALLOC_FAIL;
        return NULL;
    }
    if((tmp->marked=malloc((tmp->updated_size)*sizeof(int)))==NULL){
        free(tmp->ccindex);
        free(tmp);
        error_val=CC_MALLOC_FAIL;
        return NULL;
    }
    for(i=0;i<tmp->updated_size;i++){
        tmp->updated[i]=-1;
        tmp->marked[i]=-1;
    }
    if((tmp->UpdateIndex.uindex=malloc(tmp->updated_size*sizeof(uint32_t*)))==NULL){
        free(tmp->ccindex);
        free(tmp->updated);
        free(tmp);
        error_val=CC_MALLOC_FAIL;
        return NULL;
    }
    tmp->UpdateIndex.size = tmp->updated_size;
    return tmp;
}

rcode CC_destroy(CC_index c){
    ds_list(c->idlist);
    ds_list(c->uidlist);
    free(c->ccindex);
    free(c->updated);
    free(c->marked);
    free(c->vals);
    free(c->UpdateIndex.uindex);
    free(c);
    error_val=OK_SUCCESS;
    return OK_SUCCESS;
}

rcode CC_insertNewEdge(CC_index c,uint32_t nodeida,uint32_t nodeidb){
    uint32_t max=nodeidb;
    (nodeida>nodeidb) ? (max=nodeida) : (max=nodeidb);
    if(max>=c->index_size){
        int prev_size=c->index_size;
        int next_size=c->index_size,i;
        while(next_size<=max){
            next_size<<=1;
        }
        c->index_size=next_size;
        if((c->ccindex=realloc(c->ccindex,next_size*sizeof(uint32_t)))==NULL){
            error_val=CC_REALLOC_FAIL;
            return CC_REALLOC_FAIL;
        }
        for(i=prev_size;i<next_size;i++){
            c->ccindex[i]=-1;
        }
    }
    if(c->ccindex[nodeida]==-1 && c->ccindex[nodeidb]==-1 ){
        c->ccindex[nodeida]=c->next_component_num;
        c->ccindex[nodeidb]=c->next_component_num;
        c->next_component_num++;
        if(c->updated_size==c->next_component_num){
            int i;
            c->updated=realloc(c->updated,c->updated_size*2*(sizeof(int)));
            c->marked=realloc(c->marked,c->updated_size*2*(sizeof(int)));
            c->vals=realloc(c->vals,c->updated_size*2*(sizeof(int)));
            c->UpdateIndex.uindex=realloc(c->UpdateIndex.uindex,c->UpdateIndex.size*2*sizeof(phead));
            for(i=c->updated_size;i<c->updated_size*2;i++){
                c->updated[i]=-1;
                c->marked[i]=-1;
            }
            c->updated_size*=2;
        }
    }
    else if(c->ccindex[nodeida]==-1||c->ccindex[nodeidb]==-1){
        if(c->ccindex[nodeida]==-1){
            c->ccindex[nodeida]=c->ccindex[nodeidb];
        }
        if(c->ccindex[nodeidb]==-1){
            c->ccindex[nodeidb]=c->ccindex[nodeida];
        }
    }
    else if(!same_component_edge(c,nodeida,nodeidb)){
        if(c->updated[c->ccindex[nodeida]]<c->version){
            c->updated[c->ccindex[nodeida]]=c->version;
            c->UpdateIndex.uindex[c->ccindex[nodeida]]=get_alist(c->lists);
        }
        if(c->updated[c->ccindex[nodeidb]]<c->version){
            c->updated[c->ccindex[nodeidb]]=c->version;
            c->UpdateIndex.uindex[c->ccindex[nodeidb]]=get_alist(c->lists);
        }
        insert_back(c->UpdateIndex.uindex[c->ccindex[nodeida]],c->ccindex[nodeidb]);
        insert_back(c->UpdateIndex.uindex[c->ccindex[nodeidb]],c->ccindex[nodeida]);
    }
    error_val=OK_SUCCESS;
    return error_val;
}

int same_component_edge(CC_index c, uint32_t  nodeida,uint32_t nodeidb){
    if(c->ccindex[nodeidb]==c->ccindex[nodeida]){
        return 1;
    }
    else if(c->updated[c->ccindex[nodeida]]<c->version || c->updated[c->ccindex[nodeidb]]<c->version){
        return 0;
    }
    else{
        c->update_queries++;
        int sizea=get_size(c->UpdateIndex.uindex[c->ccindex[nodeida]]);
        int sizeb=get_size(c->UpdateIndex.uindex[c->ccindex[nodeidb]]);
        if(sizea<sizeb){
            return in(c->UpdateIndex.uindex[c->ccindex[nodeida]],c->ccindex[nodeidb]);
        }
        else{
            return in(c->UpdateIndex.uindex[c->ccindex[nodeidb]],c->ccindex[nodeida]);
        }
    }
}

int CC_same_component(CC_index c, uint32_t a, uint32_t b){
    c->queries++;
    c->metric--;
    if(c->metric==0){
        /*c->metric=(double)c->queries/(double)c->update_queries;*/
        c->metric=100;
        CC_rebuildIndexes(c);
        c->queries=0;
        c->update_queries=0;
    }
    if(c->ccindex[a]==c->ccindex[b]){
        return 1;
    }
    else{
        return CC_findNodeConnectedComponentID(c,a) == CC_findNodeConnectedComponentID(c,b);
    }
}

int CC_findNodeConnectedComponentID(CC_index c,uint32_t nodeid){
    int a,b;
    a=c->updated[c->ccindex[nodeid]];
    b=c->version;
    if(a<b){
        return c->ccindex[nodeid];
    }
    else{
        c->update_queries++;
        int min=c->ccindex[nodeid];
        insert_back(c->idlist,c->ccindex[nodeid]);
        while(get_size(c->idlist)!=0){
            int tmp = peek(c->idlist),data;
            pop_front(c->idlist);
            iterator it;
            it=ret_iterator(c->UpdateIndex.uindex[tmp]);
            while(it!=-1){
                /*printf("%d\n",data);*/
                data=get_iterator_data(c->UpdateIndex.uindex[tmp],it);
                if(c->marked[data]<c->check){
                    if(data<min){
                        min=data;
                    }
                    c->marked[data]=c->check;
                    insert_back(c->idlist,data);
                }
                it=advance_iterator(c->UpdateIndex.uindex[tmp],it);
            }
        }
        empty_list(c->idlist);
        c->check++;
        return min;
    }
}


rcode CC_rebuildIndexes(CC_index c){
    int j,tmp;
    for(j=0;j<c->updated_size;j++){
        if(c->updated[j]==c->version && c->marked[j]<c->check){
            int min=j,data;
            insert_back(c->idlist,j);
            insert_back(c->uidlist,j);
            while(get_size(c->idlist)!=0){
                tmp = peek(c->idlist);
                pop_front(c->idlist);
                iterator it;
                it=ret_iterator(c->UpdateIndex.uindex[tmp]);
                while(it!=-1){
                    data=get_iterator_data(c->UpdateIndex.uindex[tmp],it);
                    if(c->marked[data]<c->check){
                        if(data<min){
                            min=data;
                        }
                        c->marked[data]=c->check;
                        insert_back(c->idlist,data);
                        insert_back(c->uidlist,data);
                    }
                    it=advance_iterator(c->UpdateIndex.uindex[tmp],it);
                }
            }
            empty_list(c->idlist);
            while(get_size(c->uidlist)!=0){
                tmp = peek(c->uidlist);
                c->vals[tmp]=min;
                pop_front(c->uidlist);
            }
            empty_list(c->uidlist);
        }
    }
    for(j=0;j<c->index_size;j++){
        if(c->ccindex[j]!=-1 && c->updated[c->ccindex[j]]==c->version){
            c->ccindex[j]=c->vals[c->ccindex[j]];
        }
    }
    c->check++;
    c->version++;
    empty_lists(c->lists);
    error_val=OK_SUCCESS;
    return OK_SUCCESS;
}
