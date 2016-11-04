#ifndef _HASHT_
#define _HASHT_
#include "error.h"
#include <stdint.h>

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

phash create_hashtable(int hash_table_size,int(*h)(void *,void*),int type);       //type 0 for simple 1 for sorted lists
rcode h_insert(phash a,uint32_t data,uint32_t tag);                                      //inserts data
int in_hash(phash a,uint32_t data);                                         //checks if data  exists in hash
int h_delete(phash a,uint32_t data);                                        //deletes node with data
rcode ds_hash(phash a);                                                     //destroys hash table a
rcode empty_hash(phash a);                                                  //removes all elements from hash table
int ret_tag(phash a,uint32_t data);                                        //returns previous node of data

#endif
