#include <stdio.h>

#include "graph.h"
#include "error.h"

int main(int argc, char *argv[])
{
	int choice=-1;
    int node1, node2, ret_val;
    pGraph testgraph = gCreateGraph();
    while(choice!=0){
        printf("1 add node\n2 query\n3 print\n");
        scanf("%d",&choice);
        switch(choice){
            case 1:
                scanf("%d %d",&node1, &node2);
                gAddEdge(testgraph,node1, node2);
                break;
            case 2:
                scanf("%d %d",&node1, &node2);
                ret_val = gFindShortestPath(testgraph, node1, node2, BIDIRECTIONAL_BFS);
                if (ret_val < 0) print_error();
                else printf("Shortest path length: %d\n", ret_val);
                break;
            case 3:
                gPrintGraph(testgraph);
                break;
        }
    }
	return 0;
}
