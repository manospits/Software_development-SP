#ifndef _INT_LIST_
#define _INT_LIST_
#define QUEUE_INIT_SIZE 10000
#include "error.h"
#include <stdint.h>


typedef struct node * pnode;
typedef struct head * phead;

//CREATION
phead cr_list();

//DESTRUCTION
rcode ds_list(phead list_to_destroy);
rcode empty_list(phead list_to_empty);
//MODIFY
rcode insert(phead listh, uint32_t data);              //inserts node in front
rcode insert_back(phead listh,uint32_t data);          //inserts node in back
rcode pop_front(phead listh);                         //deletes first node in list


//ACCESS
int in(const phead listh,uint32_t data);          //checks if there is an element in the listh
int get_size(const phead listh);                  //get number of elements
int get_data(const pnode nd);                     //returns data
int peek(const phead listh);                 //returns list's first int



#endif
