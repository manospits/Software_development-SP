#ifndef _LIST_
#define _LIST_
#define QUEUE_INIT_SIZE 500000
#include "error.h"
#include <stdint.h>
#include <stdlib.h>


typedef struct ghead * gphead;

//CREATION
gphead gcr_list(size_t data_size,rcode(*data_copy)(char*,void*),int(*data_cmp)(void*,void*));
//DESTRUCTION
rcode gds_list(gphead list_to_destroy);
rcode gempty_list(gphead list_to_empty);
//MODIFY
rcode ginsert_back(phead listh,void * data);          //inserts node in back
rcode gpop_front(phead listh);                         //deletes first node in list
rcode gpop_back(phead listh);


//ACCESS
int gget_size(const gphead listh);                  //get number of elements
void * gget_data(void *data);                     //returns data
void * gpeek(const gphead listh);                 //returns list's first int
rcode int_copy(char*a,void*b);
int int_cmp(void*a,void*b);


#endif
