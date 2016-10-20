#ifndef _INTLIST_
#define _INTLIST_

typedef struct node * pnode;
typedef struct head * phead;

//CREATION
phead cr_list();
pnode cr_node(int data);

//DESTRUCTION
rcode ds_list(phead list_to_destroy);
rcode ds_node(pnode node_to_destroy);

//MODIFY
rcode insert(phead listh, int data);              //inserts node in front
rcode insert_sorted(phead listh,int data);        //inserts node sorted
rcode insert_back(phead listh,int data);          //inserts node in back
rcode delete(phead listh,int data);               //deletes node with data = data
rcode  pop_back(phead listh);                      //deletes last node in list

//ACCESS
int in(const phead listh,int data);          //checks if there is an element in the listh
int ins(const phead listh,int data);         //checks if there is an element in the sorted listh
int get_size(const phead listh);             //get number of elements
int get_data(const pnode nd);                //returns a ptr to data
int peek(const phead listh);                 //returns list's first int
int peekback(const phead listh);             //returns list's last int

//SPECIFIC ACCESS
pnode get_list(const phead listh);           //returns ptr to the first node in list
pnode next_node(const pnode nd);             //retuns next node of pnode
pnode get_node(phead listh,void* data);      //returns node with data

#endif
