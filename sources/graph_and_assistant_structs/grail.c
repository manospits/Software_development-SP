#include "../include/grail.h"
#include "../include/scc.h"
#include "../include/intlist.h"
#include "../include/error.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

typedef struct Gnode{
    uint32_t rank[LABEL_NUMBER];
    uint32_t min_rank[LABEL_NUMBER];
}Gnode;

typedef struct GrailIndex{
    Gnode *gindex;
    int gindex_size;
}GrailIndex;

Grail buildGrailIndex(pSCC s,phead nodes,phead nodesp){
    Grail g;
    int *visited;
    int visited_version=1;
    int i,k,l,posa,posb;
    if((g=malloc(sizeof(GrailIndex)))==NULL){
        print_errorv(GRAIL_MALLOC_FAIL);
        /*error_val=GRAIL_MALLOC_FAIL;*/
        return NULL;
    }
    g->gindex_size=get_number_of_components(s);
    if((g->gindex=malloc(g->gindex_size*sizeof(struct Gnode)))==NULL){
        free(g);
        print_errorv(GRAIL_MALLOC_FAIL);
        /*error_val=GRAIL_MALLOC_FAIL;*/
        return NULL;
    }
    if((visited=malloc(g->gindex_size*sizeof(int)))==NULL){
        free(g);
        print_errorv(GRAIL_MALLOC_FAIL);
        /*error_val=GRAIL_MALLOC_FAIL;*/
        return NULL;
    }
    for(i=0;i<g->gindex_size;i++){
        for(k=0;k<LABEL_NUMBER;k++)
            g->gindex[i].min_rank[k]=16777216;
        visited[i]=-1;
    }
    //ITERATIVE
    srand(time(NULL));
    uint32_t rank=0 ;
    uint32_t nodeid,parent,*neighbors,size,*neighbors_copy=NULL,neighbors_copy_size=0,temp;
    for(l=0;l<LABEL_NUMBER;l++){
        nodeid=rand() % g->gindex_size;
        parent=nodeid;
        for(k=-1;k<g->gindex_size;k++){
            if(k!=-1){
                nodeid=k;
                parent=k;
            }
            if(visited[nodeid]<visited_version){
                insert_back(nodes,nodeid);
                insert_back(nodesp,parent);
                while(get_size(nodes)!=0){
                    nodeid=peek_back(nodes);
                    parent=peek_back(nodesp);
                    if(visited[nodeid]<visited_version){
                        visited[nodeid]=visited_version;
                        get_component_neighbors(s,nodeid,&neighbors,&size);
                        if(neighbors_copy==NULL){
                            int size_n=2;
                            while(size_n<size){
                                size_n<<=1;
                            }
                            if((neighbors_copy=malloc(size_n*sizeof(uint32_t)))==NULL){
                                print_errorv(GRAIL_MALLOC_FAIL);
                                /*error_val=GRAIL_MALLOC_FAIL;*/
                                return NULL;
                            }
                            neighbors_copy_size=size_n;
                        }
                        else if(neighbors_copy_size<size){
                            int size_n=neighbors_copy_size;
                            while(size_n<size){
                                size_n<<=1;
                            }
                            if((neighbors_copy=realloc(neighbors_copy,size_n*sizeof(uint32_t)))==NULL){
                                print_errorv(GRAIL_MALLOC_FAIL);
                                /*error_val=GRAIL_MALLOC_FAIL;*/
                                return NULL;
                            }
                            neighbors_copy_size=size_n;
                        }
                        memcpy(neighbors_copy,neighbors,size*sizeof(uint32_t));
                        for(i=0;i<size;i++){
                            posa=rand()%size;
                            posb=rand()%size;
                            temp=neighbors_copy[posa];
                            neighbors_copy[posa]=neighbors_copy[posb];
                            neighbors_copy[posb]=temp;
                        }
                        for(i=0;i<size;i++)
                        {
                            if(visited[neighbors_copy[i]]<visited_version){
                                insert_back(nodes,neighbors_copy[i]);
                                insert_back(nodesp,nodeid);
                            }
                            else{
                                if(g->gindex[neighbors_copy[i]].min_rank[l] < g->gindex[nodeid].min_rank[l]){
                                    g->gindex[nodeid].min_rank[l]=g->gindex[neighbors_copy[i]].min_rank[l];
                                }
                            }
                        }
                    }
                    else{
                        rank++;
                        g->gindex[nodeid].rank[l]=rank;
                        if(g->gindex[nodeid].min_rank[l]==16777216){
                            g->gindex[nodeid].min_rank[l]=rank;
                        }
                        if(g->gindex[parent].min_rank[l]>g->gindex[nodeid].min_rank[l]){
                            g->gindex[parent].min_rank[l]=g->gindex[nodeid].min_rank[l];
                        }
                        pop_back(nodes);
                        pop_back(nodesp);
                    }
                }
            }
        }
        visited_version++;
    }
    free(neighbors_copy);
    free(visited);
    return g;
}


GRAIL_ANSWER isReachableGrailIndex(GrailIndex* g,pSCC s, uint32_t source_node,uint32_t target_node){
    int i;
    uint32_t t_node;
    uint32_t s_node;
    s_node=findNodeStronglyConnectedComponentID(s, source_node);
    t_node=findNodeStronglyConnectedComponentID(s, target_node);
    for(i=0;i<LABEL_NUMBER;i++){
        if(g->gindex[t_node].min_rank[i] >= g->gindex[s_node].min_rank[i] && g->gindex[t_node].rank[i] <= g->gindex[s_node].rank[i]){
            continue;
        }
        else{
            return 0;
        }
    }
    return 1;
}

rcode destroyGrailIndex(GrailIndex * g){
    free(g->gindex);
    free(g);
    return OK_SUCCESS;
}

