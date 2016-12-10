#ifndef _HASHT_
#define _HASHT_
#include "error.h"
#include <stdint.h>
#define HASHTABLE_SIZE 20000

typedef struct hash_info *phash;
typedef struct hash_node *phnode;
// hash table
// __
//|__|->N->....->|-
//|__|->N->N->..|-
//| .|
//| .|
//| .|
//|__|->N->...|-

phash create_hashtable(int hash_table_size,int(*h)(void *,void*));       //type 0 for simple 1 for sorted lists
rcode h_insert(phash a,uint32_t data);                                      //inserts data
int in_hash(phash a,uint32_t data);                                         //checks if data  exists in hash
rcode ds_hash(phash a);                                                     //destroys hash table a
rcode empty_hash(phash a);                                                  //removes all elements from hash table


#endif
