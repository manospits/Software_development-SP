#include "index.h"
#include "error.h"
#include <stdio.h>

int main(void){
    Index_ptr index;
    int i,j;
    index=createNodeIndex();
    if(index==NULL){
        print_error();
    }
    for(i=0;i<10000;i++){
        insertNode(index,i);
        for(j=0;j<1000;j++){
            add_edge(index,i,i+j);
        }

    }
    for(i=0;i<10000;i++){
        for(j=0;j<1000;j++)
            if(edge_exists(index,i,i+j)==1){
                printf("node %d edge %d\n",i,i+j);
            }
            else{
                puts("hmmm");
            }
    }

    destroyNodeIndex(index);
}
