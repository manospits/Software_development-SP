#ifndef _STRUCT_LIST_
#define _STRUCT_LIST_
#include "error.h"
#include <stdint.h>

typedef struct stnode * stpnode;
typedef struct sthead * stphead;

//CREATION
stphead st_cr_list();

//DESTRUCTION
rcode st_ds_list(stphead list_to_destroy);

//MODIFY
rcode st_insert(stphead listh, uint32_t data,uint32_t tag);              //inserts node in front
rcode st_insert_sorted(stphead listh,uint32_t data,uint32_t tag);        //inserts node sorted
rcode st_insert_back(stphead listh,uint32_t data,uint32_t tag);          //inserts node in back
rcode st_delete(stphead listh,uint32_t data);                             //deletes node with data = data
rcode st_pop_back(stphead listh);                                         //deletes last node in list

//ACCESS
int st_in(const stphead listh,uint32_t data);          //checks if there is an element in the listh
int st_ins(const stphead listh,uint32_t data);         //checks if there is an element in the sorted listh
int st_get_size(const stphead listh);                  //get number of elements
int st_get_data(const stpnode nd);                     //returns data
int st_get_tag(const stphead listh,uint32_t data);    //returns tag value of node
int st_peek(const stphead listh);                 //returns list's first int
int st_peekback(const stphead listh);             //returns list's last int

//SPECIFIC ACCESS
stpnode st_get_list(const stphead listh);           //returns ptr to the first node in list
stpnode st_next_node(const stpnode nd);             //retuns next node of stpnode

#endif
