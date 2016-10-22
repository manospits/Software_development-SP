#include "error.h"
#include <stdio.h>

int error_val=OK_SUCCESS;

void print_error(){
    switch(error_val){
        case OK_SUCCESS:
            perror("No error from structures.\n");
            error_val=OK_SUCCESS;
            break;
        case LIST_CR_MALLOC:
            perror("Error allocating memory in list creation\n");
            error_val=OK_SUCCESS;
            break;
        case LISTNODE_CR_MALLOC:
            perror("Error allocating memory in list node creation\n");
            error_val=OK_SUCCESS;
            break;
        case NULL_LIST:
            perror("Error: list head called in function with NULL value\n");
            error_val=OK_SUCCESS;
            break;
        case NULL_NODE:
            perror("Error: list node called in function with NULL value\n");
            error_val=OK_SUCCESS;
            break;
        case HASH_CR_MALLOC:
            perror("Error allocating memory in hash table creation\n");
            error_val=OK_SUCCESS;
            break;
        case HASH_BINS_CR_MALLOC:
            perror("Error allocating memory in hash table bins creation\n");
            error_val=OK_SUCCESS;
            break;
        case HASH_WRONG_PARMS:
            fprintf(stderr, "Error wrong parameter in hash table creation:\nvalues must be ----size > 0----type = 1 || 0----\n");
            error_val=OK_SUCCESS;
            break;
        case NULL_HASH:
            perror("Error: hash table called in function with NULL value\n");
            error_val=OK_SUCCESS;
            break;
        case BUFFER_CR_MALLOC:
            perror("Error allocating struct Buffer\n");
            error_val=OK_SUCCESS;
            break;
        case BUFFER_CR_SPACE_MALLOC:
            perror("Error allocating memory space in struct Buffer\n");
            error_val=OK_SUCCESS;
            break;
        case BUFFER_DOUBLE_SIZE:
            perror("Error reallocating space and doubling size\n");
            error_val=OK_SUCCESS;
            break;
        case BUFFER_PTR_OUT_BOUNDS:
            perror("Error given ptr value exceeds the permitted values\n");
            error_val=OK_SUCCESS;
            break;
        case NULL_BUFFER:
            perror("Error Buffer pointer is NULL\n");
            error_val=OK_SUCCESS;
            break;
        case NULL_BUFFER_SPACE:
            perror("Error Buffer allocated space pointer points to NULL location\n");
            error_val=OK_SUCCESS;
            break;
    }
}
