#ifndef _VISITED_
#define _VISITED_
#include "error.h"
#include <stdint.h>

typedef struct visited_head *pvis;
typedef struct visited_node *pvnode;

pvis create_visited(int size);
rcode v_mark(pvis a,uint32_t data,int tag,int expanded);    //mark data
int v_visited(pvis a,uint32_t data);                        //check if data is visited
rcode v_update_loop(pvis a,uint32_t size);                  //update loop for next checks (updates version)
rcode v_ds_visited(pvis a);                                 //destroy visited structure
int v_ret_tag(pvis a,uint32_t data);                        //return tag of data
int v_ret_expanded(pvis a,uint32_t data);                   //return expanded value of data
rcode v_set_expanded(pvis a,uint32_t data,int expanded);    //set expanded value of data

#endif
