#ifndef _HASHT_
#define _HASHT_

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

phash create_hashtable(int hash_table_size,int(*h)(void *),int type); //type 0 for simple 1 for sorted lists
phnode create_phnode(void* el);                                       //creates a hash_node
int h_insert(phash a,int data);                                       //inserts data
int in_hash(phash a,int data);                                        //checks if data  exists in hash
int h_delete(phash a,int data);                                       //deletes node with data
void ds_hash(phash a);                                                //destroys hash table a

#endif
