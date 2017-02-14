#ifndef _INT_LIST_
#define _INT_LIST_
#define QUEUE_INIT_SIZE 10000
#include "error.h"
#include <stdint.h>

typedef int iterator;
typedef struct node * pnode;
typedef struct head * phead;

//CREATION
phead cr_list();

//DESTRUCTION
rcode ds_list(phead list_to_destroy);                //destroys list
rcode empty_list(phead list_to_empty);               //remove all elements from list (doesnt destroy list structure)
//MODIFY
rcode insert_back(phead listh,uint32_t data);        //inserts node in back
rcode pop_front(phead listh);                        //deletes first node in list
rcode pop_back(phead listh);                         //deletes first node in list


//ACCESS
int in(const phead listh,uint32_t data);          //checks if there is an element in the listh
int get_size(const phead listh);                  //get number of elements
int* get_sizep(const phead listh);                //get number of elements
int get_data(const pnode nd);                     //returns data
int peek(const phead listh);                      //returns list's first int
uint32_t peek_pop_front(const phead listh);                 //returns list's first int
int peek_back(const phead listh);                 //returns list's first int
iterator ret_iterator(const phead listh);         //returns an iterator to the called list
int get_iterator_data(const phead listh,iterator it);       //returns iterator's data of list listh
iterator advance_iterator(const phead listh,iterator it);   //advances iterator ,if end of list is reached iterator gets -1 value


#endif
