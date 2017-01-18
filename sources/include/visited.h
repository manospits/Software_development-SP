#ifndef _VISITED_
#define _VISITED_
#include "error.h"
#include <stdint.h>

typedef struct visited_head *pvis;
typedef struct visited_node *pvnode;

pvis create_visited(int size);
rcode v_mark(pvis a,uint32_t data,int tag,int expanded);    //inserts data
int v_visited(pvis a,uint32_t data);                       //checks if data  exists in hash
rcode v_update_loop(pvis a,uint32_t size);
rcode v_ds_visited(pvis a);
int v_ret_tag(pvis a,uint32_t data);                                        //returns previous node of data
int v_ret_expanded(pvis a,uint32_t data);
rcode v_set_expanded(pvis a,uint32_t data,int expanded);

#endif
