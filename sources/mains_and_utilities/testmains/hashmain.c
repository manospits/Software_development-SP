#include "hash.h"
#include <stdio.h>
int hash_size=10;
int h(void* data){
    return (*(int*)data)%hash_size;
}
int main(void){
    int choice=-1;
    int num=0,prev;
    phash hasht;
    while(choice!=0){
        printf("1 insert\n2 delete\n");
        printf("3 in\n4 create simple\n5 create sorted\n6 destroy\n7 get prev\n");
        scanf("%d",&choice);
        switch(choice){
            case 1:
                scanf("%d %d",&num,&prev);
                h_insert(hasht,num,prev);
                break;
            case 2:
                scanf("%d",&num);
                h_delete(hasht,num);
                break;
            case 3:
                scanf("%d",&num);
                if(in_hash(hasht,num)){
                    puts("yes");
                }
                else puts("no");
                break;
            case 4:
                hasht=create_hashtable(hash_size,h,0);
                break;
            case 5:
                hasht=create_hashtable(hash_size,h,1);
                break;
            case 6:
                ds_hash(hasht);
                break;
            case 7:
                scanf("%d",&num);
                printf("prev of %d is %d\n",num,ret_tag(hasht,num));
                break;

        }
    }
}
