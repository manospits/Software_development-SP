#ifndef _QUERIES_
#define _QUERIES_
#define QLIST_INIT_SIZE 10000
#include "error.h"
#include <stdint.h>

typedef struct querie{
    uint32_t querie_id;
    uint32_t nodea;
    uint32_t nodeb;
    long unsigned int version;
}querie;

typedef struct querie * qpnode;
typedef struct qhead * qphead;

//CREATION
qphead q_cr_list();

//DESTRUCTION
rcode q_ds_list(qphead list_to_destroy);
rcode q_empty_list(qphead list_to_empty);
//MODIFY
rcode q_insert_back(qphead listh,uint32_t querie_id,uint32_t nodea,uint32_t nodeb,long unsigned int version);          //inserts node in back
rcode q_pop_front(qphead listh);                         //deletes first node in list

//ACCESS
int q_get_size(const qphead listh);                     //get number of elements
qpnode q_peek(const qphead listh);                      //returns list's first querie
qpnode q_peek_back(const qphead listh);                    //returns list's first int



#endif
