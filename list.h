#ifndef _LIST_
#define _LIST_
#define QUEUE_INIT_SIZE 500000
#include "error.h"
#include <stdint.h>
#include <stdlib.h>


typedef struct head * phead;

//CREATION
phead cr_list(size_t data_size,rcode(*data_copy)(char*,void*),int(*data_cmp)(void*,void*));
//DESTRUCTION
rcode ds_list(phead list_to_destroy);
rcode empty_list(phead list_to_empty);
//MODIFY
rcode insert_back(phead listh,void * data);          //inserts node in back
rcode pop_front(phead listh);                         //deletes first node in list
rcode pop_back(phead listh);


//ACCESS
int get_size(const phead listh);                  //get number of elements
void * get_data(void *data);                     //returns data
void * peek(const phead listh);                 //returns list's first int
rcode int_copy(char*a,void*b);
int int_cmp(void*a,void*b);


#endif
