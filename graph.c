#include <stdlib.h>
#include "graph.h"
#include "buffer.h"
#include "index.h"
#include "hash.h"
#include "intlist.h"

typedef struct graph
{
	Index_ptr inIndex;
	pBuffer inBuffer;
	Index_ptr outIndex;
	pBuffer outBuffer;
}graph;

rcode gCreateGraph(pGraph *g)
{

}

rcode gAddNode(pGraph g, graphNode from, graphNode to)
{

}

rcode gFindShortestPath(pGraph g, graphNode from, graphNode to/*, &solution*/)
{

}

rcode gDestroyGraph(pGraph g)
{

}
