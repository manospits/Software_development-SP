#ifndef _INTLIST_
#define _INTLIST_
#include "error.h"
#include <stdint.h>

typedef struct node * pnode;
typedef struct head * phead;

//CREATION
phead cr_list();

//DESTRUCTION
rcode ds_list(phead list_to_destroy);

//MODIFY
rcode insert(phead listh, uint32_t data,uint32_t tag);              //inserts node in front
rcode insert_sorted(phead listh,uint32_t data,uint32_t tag);        //inserts node sorted
rcode insert_back(phead listh,uint32_t data,uint32_t tag);          //inserts node in back
rcode delete(phead listh,uint32_t data);                             //deletes node with data = data
rcode pop_back(phead listh);                                         //deletes last node in list

//ACCESS
int in(const phead listh,uint32_t data);          //checks if there is an element in the listh
int ins(const phead listh,uint32_t data);         //checks if there is an element in the sorted listh
int get_size(const phead listh);                  //get number of elements
int get_data(const pnode nd);                     //returns data
int get_tag(const phead listh,uint32_t data);    //returns tag value of node
int peek(const phead listh);                 //returns list's first int
int peekback(const phead listh);             //returns list's last int

//SPECIFIC ACCESS
pnode get_list(const phead listh);           //returns ptr to the first node in list
pnode next_node(const pnode nd);             //retuns next node of pnode

#endif
