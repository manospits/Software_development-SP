#ifndef _QUERIES_
#define _QUERIES_
#define QLIST_INIT_SIZE 10000
#include "error.h"
#include <stdint.h>

typedef struct query{
    uint32_t version;
    uint32_t query_id;
    uint32_t nodea;
    uint32_t nodeb;
}query;

typedef struct query * qpnode;
typedef struct qhead * qphead;

//CREATION
qphead q_cr_list();

//DESTRUCTION
rcode q_ds_list(qphead list_to_destroy);
rcode q_empty_list(qphead list_to_empty);
//MODIFY
rcode q_insert_back(qphead listh,uint32_t query_id,uint32_t nodea,uint32_t nodeb,long unsigned int version);          //inserts querie in back
rcode q_pop_front(qphead listh);                         //deletes first querie in list

//ACCESS
int q_get_size(const qphead listh);                     //get number of queries
qpnode q_peek(const qphead listh);                      //returns list's front querie
qpnode q_peek_back(const qphead listh);                 //returns querie list's back querie



#endif
