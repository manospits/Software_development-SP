/*
 * DO NOT EDIT THIS FILE. Generated by checkmk.
 * Edit the original source file "testmain.check" instead.
 */

#include <check.h>

#line 1 "testmain.check"
#include "intlist.h"
#include "struct_list.h"
#include "queries.h"
#include "index.h"
#include "error.h"
#include <stdio.h>

int hash_function(void *data,void *size)
{
    return (*(uint32_t *)data) % (*(int*)size);
}

START_TEST(intlist)
{
#line 14
    phead list;
    int i,j,k;
    for(i=0;i<3;i++){
        fail_unless((list=cr_list())!=NULL);
        for(j=0;j<1000;j++){
            fail_unless(insert_back(list,j)==OK_SUCCESS);
            fail_unless(peek_back(list)==j);
        }
        fail_unless(get_size(list)==1000);
        for(k=0;k<1100;k++){
            if(k<1000){
                fail_unless(in(list,k));
            }
            else{
                fail_unless(!in(list,k));
            }
        }
        fail_unless(ds_list(list)==OK_SUCCESS);
    }
    puts("testing intlist finished");
}
END_TEST

START_TEST(struct_list)
{
#line 35
    stphead list;
    int i,j,k;
    for(i=0;i<3;i++){
        fail_unless((list=st_cr_list())!=NULL);
        for(j=0;j<1000;j++){
            fail_unless(st_insert_back(list,j,j,i)==OK_SUCCESS);
            fail_unless(st_peek_back(list)==j);
        }
        fail_unless(st_get_size(list)==1000);
        for(k=0;k<1100;k++){
            if(k<1000){
                fail_unless(st_in(list,k));
                fail_unless(st_get_expanded(list,k)==i);
                fail_unless(st_set_expanded(list,k,i+1)==OK_SUCCESS);
                fail_unless(st_get_expanded(list,k)==i+1);
            }
            else{
                fail_unless(!st_in(list,k));
            }
        }
        fail_unless(st_ds_list(list)==OK_SUCCESS);
    }
    puts("testing structlist finished");
}
END_TEST

START_TEST(queries)
{
#line 59
    qphead list;
    int i,j;
    querie *tmp;
    for(i=0;i<3;i++){
        fail_unless((list=q_cr_list())!=NULL);
        for(j=0;j<1000;j++){
            fail_unless(q_insert_back(list,j,j,i,i)==OK_SUCCESS);
            fail_unless((tmp=q_peek_back(list))!=NULL);
            fail_unless(tmp->querie_id==j);
            fail_unless(tmp->nodea==j);
            fail_unless(tmp->nodeb==i);
            fail_unless(tmp->version==i);
        }
        fail_unless(q_get_size(list)==1000);
        fail_unless(q_ds_list(list)==OK_SUCCESS);
    }
    puts("testing queries finished");

}
END_TEST

START_TEST(test_serial)
{
#line 78
    Index_ptr index;
    int i,j;
    fail_unless((index=createNodeIndex())!=NULL);
    for(i=0;i<1000;i++){
        fail_unless(insertNode(index,i)==OK_SUCCESS);
        for(j=0;j<100;j++){
            if(j%2==0)
                fail_unless(add_edge(index,i,i+j)==OK_SUCCESS);
        }
    }
    for(i=0;i<1000;i++)
        for(j=0;j<100;j++){
            if(j%2==0)
                fail_unless(edge_exists(index,i,i+j)==1);
            else
                fail_unless(edge_exists(index,i,i+j)==0);
        }
    fail_unless(destroyNodeIndex(index)==OK_SUCCESS);
    puts("test with 1000 serial inserts and 100 neighbors finished ");
}
END_TEST

START_TEST(test_step10)
{
#line 98
    Index_ptr index;
    int i,j,start;
    fail_unless((index=createNodeIndex())!=NULL);
    for(start=0;start<10;start++){
        for(i=start;i<1000;i+=10){
            fail_unless(insertNode(index,i)==OK_SUCCESS);
            for(j=0;j<100;j++){
                if(j%2==0)
                    fail_unless(add_edge(index,i,i+j)==OK_SUCCESS);
            }
        }
    }
    for(start=0;start<10;start++){
        for(i=start;i<1000;i+=10)
            for(j=0;j<100;j++){
                if(j%2==0)
                    fail_unless(edge_exists(index,i,i+j)==1);
                else
                    fail_unless(edge_exists(index,i,i+j)==0);
            }
    }
    fail_unless(destroyNodeIndex(index)==OK_SUCCESS);
    puts("test with 1000 inserts step 10 and 100 neighbors finished ");
}
END_TEST

int main(void)
{
    Suite *s1 = suite_create("Core");
    TCase *tc1_1 = tcase_create("Core");
    SRunner *sr = srunner_create(s1);
    int nf;

    suite_add_tcase(s1, tc1_1);
    tcase_add_test(tc1_1, intlist);
    tcase_add_test(tc1_1, struct_list);
    tcase_add_test(tc1_1, queries);
    tcase_add_test(tc1_1, test_serial);
    tcase_add_test(tc1_1, test_step10);

    srunner_run_all(sr, CK_ENV);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? 0 : 1;
}
