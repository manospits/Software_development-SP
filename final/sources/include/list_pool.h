#ifndef _LIST_POOL_
#define _LIST_POOL_
#include "struct_list.h"
#define LPOOL_INIT_SIZE 2

typedef struct list_pool * lpool;

lpool create_listpool();    //create a list pool structure (a pool of lists!)
stphead get_alist(lpool l); //get a list pointer from the list pool
rcode empty_lists(lpool l); //empty all lists in list pool
rcode ds_pool(lpool l);     //destroy list pool and allocated list

#endif
