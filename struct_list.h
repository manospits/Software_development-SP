#ifndef _STRUCT_LIST_
#define _STRUCT_LIST_
#define STQUEUE_INIT_SIZE 10000
#include "error.h"
#include <stdint.h>


typedef struct stnode * stpnode;
typedef struct sthead * stphead;

//CREATION
stphead st_cr_list();

//DESTRUCTION
rcode st_ds_list(stphead list_to_destroy);
rcode st_empty_list(stphead list_to_empty);
//MODIFY
rcode st_insert(stphead listh, uint32_t data,uint32_t tag,int expanded);              //inserts node in front
rcode st_insert_back(stphead listh,uint32_t data,uint32_t tag,int expanded);          //inserts node in back
rcode st_pop_front(stphead listh);                         //deletes first node in list
rcode st_set_expanded(const stphead listh,uint32_t data,int expanded);

//ACCESS
int st_in(const stphead listh,uint32_t data);          //checks if there is an element in the listh
int st_get_size(const stphead listh);                  //get number of elements
int st_get_data(const stpnode nd);                     //returns data
int st_get_tag(const stphead listh,uint32_t data);    //returns tag value of node
int st_get_expanded(const stphead listh,uint32_t data);    //returns tag value of node
int st_peek(const stphead listh);                 //returns list's first int



#endif
