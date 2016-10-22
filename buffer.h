#ifndef _BUFFER_
#define _BUFFER_
#include "error.h"
#include <stdint.h>

#ifndef N
#define N 10 //array size in nodes
#endif

typedef int ptr;
typedef struct Buffer* pBuffer;

typedef struct list_node{
    uint32_t neighbor[N];       //the ids of the neighbor nodes
    uint32_t edgeProperty[N];   //property for each edge
    ptr  nextListNode;          //
}lnode;

typedef lnode* plnode;

pBuffer createBuffer();             //ALLOCATES AND SETS NEW BUFFER
                                    //STRUCT IN CASE OF ERROR RETURN NULL

ptr allocNewNode(pBuffer);          //ALLOCATES NEW NODE IN BUFFER SPACE
                                    //AND RETURNS PTR (OFFSET) TO THE NODE CREATED
                                    //IN CASE OF ERROR NEGATIVE VALUE IS RETURNED

plnode getListNode(pBuffer,ptr);    //RETURNS POINTER TO LNODE STRUCT GIVEN PTR AND BUFFER
                                    //IN CASE OF ERROR NULL IS RETURNED

rcode destroyBuffer(pBuffer);       //DESTROYS BUFFER AND RETURNS SUCCESS OR ERROR_CODE

#endif
