#include "intlist.h"
#include <stdio.h>

int main(void){
    int choice=-1;
    pnode tmp;
    int num=0;
    phead list;
    while(choice!=0){
        printf("1 insert\n2 insert sorted\n3 insert back\n4 delete\n5 pop back\n");
        printf("6 in\n7 ins\n8 size\n9 peek\n10 peek back\n11 create\n12 destroy\n13 print\n14 pop front\n");
        scanf("%d",&choice);
        switch(choice){
            case 1:
                scanf("%d",&num);
                insert(list,num);
                break;
            case 2:
                scanf("%d",&num);
                insert_sorted(list,num);
                break;
            case 3:
                scanf("%d",&num);
                insert_back(list,num);
                break;
            case 4:
                scanf("%d",&num);
                delete(list,num);
                break;
            case 5:
                pop_back(list);
                break;
            case 14:
                pop_front(list);
                break;
            case 6:
                scanf("%d",&num);
                if(in(list,num)){
                    puts("yes");
                }
                else puts("no");
                break;
            case 7:
                scanf("%d",&num);
                if(ins(list,num)){
                    puts("yes");
                }
                else puts("no");
                break;
            case 8:
                printf("size :%d\n",get_size(list));
                break;
            case 9:
                printf("front :%d\n",peek(list));
                break;
            case 10:
                printf("front :%d\n",peekback(list));
                break;
            case 11:
                list=cr_list();
                break;
            case 12:
                ds_list(list);
                break;
            case 13:
                tmp=get_list(list);
                while(tmp!=NULL){
                    printf("%d ",get_data(tmp));
                    tmp=next_node(tmp);
                }
                puts("");
                break;
        }
    }
}
