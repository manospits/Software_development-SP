#include "grail.h"
#include "scc.h"
#include "intlist.h"
#include "error.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
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
    puts("ok");
    Grail g;
    int *visited;
    int visited_version=1;
    int i,k,l;
    if((g=malloc(sizeof(GrailIndex)))==NULL){
        error_val=GRAIL_MALLOC_FAIL;
        return NULL;
    }
    g->gindex_size=get_number_of_components(s);
    if((g->gindex=malloc(g->gindex_size*sizeof(struct Gnode)))==NULL){
        free(g);
        error_val=GRAIL_MALLOC_FAIL;
        return NULL;
    }
    if((visited=malloc(g->gindex_size*sizeof(int)))==NULL){
        free(g);
        error_val=GRAIL_MALLOC_FAIL;
        return NULL;
    }
    for(i=0;i<g->gindex_size;i++){
        for(k=0;k<LABEL_NUMBER;k++)
            g->gindex[i].min_rank[k]=0;
        visited[i]=0;
    }
    //ITERATIVE
    srand(time(NULL));
    int rank=0 ;
    uint32_t nodeid,parent,*neighbors=NULL,size;
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
                        neighbors=get_component_neighbors(s,nodeid,&size);
                        for(i=0;i<size;i++)
                        {
                            if(visited[neighbors[i]]<visited_version){
                                insert_back(nodes,neighbors[i]);
                                insert_back(nodesp,nodeid);
                            }
                            else{
                                if(g->gindex[neighbors[i]].min_rank[l] < g->gindex[nodeid].min_rank[l]){
                                    g->gindex[nodeid].min_rank[l]=g->gindex[neighbors[i]].min_rank[l];
                                }
                            }
                        }
                    }
                    else{
                        rank++;
                        if(g->gindex[nodeid].min_rank==0){
                            g->gindex[nodeid].min_rank[l]=rank;
                        }
                        if(g->gindex[parent].min_rank[l]>g->gindex[nodeid].min_rank[l]){
                            g->gindex[parent].min_rank[l]=g->gindex[nodeid].min_rank[l];
                        }
                        g->gindex[nodeid].rank[l]=rank;
                        pop_back(nodes);
                        pop_back(nodesp);
                    }
                }
            }
        }
        visited_version++;
    }
    free(visited);
    puts("ok");
    return g;
}


GRAIL_ANSWER isReachableGrailIndex(GrailIndex* g, uint32_t source_node,uint32_t target_node){
    int i;
    for(i=0;i<LABEL_NUMBER;i++){
        if(g->gindex[target_node].min_rank[i] >= g->gindex[source_node].min_rank[i] && g->gindex[target_node].rank[i] <= g->gindex[source_node].rank[i]){
            return 1;
        }
    }
    return 0;
}

rcode destroyGrailIndex(GrailIndex * g){
    free(g->gindex);
    free(g);
    return OK_SUCCESS;
}

