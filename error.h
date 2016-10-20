#ifndef _ERROR_
#define _ERROR_

typedef int rcode;
void print_error();

//ERROR CODES EXPLANATION
#define OK_SUCCESS 0
#define LIST_CR_MALLOC 1
#define LISTNODE_CR_MALLOC 2
#define NULL_LIST 3
#define NULL_NODE 4
#define HASH_CR_MALLOC 5
#define HASH_BINS_CR_MALLOC 6
#define HASH_WRONG_PARMS 7
#define NULL_HASH 8

#endif
