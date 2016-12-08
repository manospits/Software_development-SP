#ifndef _LIST_POOL_
#define _LIST_POOL_
#include "intlist.h"
#define LPOOL_INIT_SIZE 2

typedef struct list_pool * lpool;

lpool create_listpool();
phead get_alist(lpool l);
rcode empty_lists(lpool l);
rcode ds_pool(lpool l);

#endif
