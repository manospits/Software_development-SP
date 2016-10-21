#include "error.h"
#include <stdint.h>
#ifndef _BUFFER__
#define _BUFFER_

typedef int ptr;
typedef struct Buffer* pBuffer;
typedef struct list_node* plnode;

pBuffer createBuffer();             //ALLOCATES AND SETS NEW BUFFER
                                    //STRUCT IN CASE OF ERROR RETURN NULL

ptr allocNewNode(pBuffer);          //ALLOCATES NEW NODE IN BUFFER SPACE
                                    //AND RETURNS PTR (OFFSET) TO THE NODE CREATED
                                    //IN CASE OF ERROR -1 IS RETURNED

plnode getListNode(pBuffer,ptr);    //RETURNS POINTER TO LNODE STRUCT GIVEN PTR AND BUFFER
                                    //IN CASE OF ERROR NULL IS RETURNED

rcode destroyBuffer(pBuffer);       //DESTROYS BUFFER AND RETURNS SUCCESS OR ERROR_CODE

#endif
