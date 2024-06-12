#include <stdio.h>
#include <stdint.h>

//#define FROM_NEXT_TO_TEST_T(n) (test_t*)((char *)n - OFFSETOF(test_t, next))

//#define CAST_OFFSET_TO_TYPE(type, next, n) n ? (type*)((char *)n - OFFSETOF(type, next)) : n
//#define CAST_TYPE_TO_OFFSET(type, next, n) n ? (void*)((char *)n + OFFSETOF(type, next)) : n

//#define FROM_NEXT_TO_TEST_T(n) CAST_OFFSET_TO_TYPE(test_t, next, n)
//#define FROM_TEST_T_TO_NEXT(n) CAST_TYPE_TO_OFFSET(test_t, next, n)

typedef struct test1_struct
{
    char data;
    struct test1_struct * next;
} test1_t;

#define TEMPLATE_PREFIX test1
#define TEMPLATE_STRUCT test1_t
#define TEMPLATE_NEXT next
#include "LinkedList.h"

typedef struct test2_struct
{
    struct test2_struct * next;
    char data;
} test2_t;

#define TEMPLATE_PREFIX test2
#define TEMPLATE_STRUCT test2_t
#define TEMPLATE_NEXT next
#include "LinkedList.h"

void test1_print(test1_t * t, void * param)
{
    if(t != NULL)
        printf("%c%s", t->data, t->next ? "->" : "\r\n");
    else
        printf("%c%s", ' ', "\r\n");
}

void test2_print(test2_t * t, void * param)
{
    if(t != NULL)
        printf("%c%s", t->data, t->next ? "->" : "\r\n");
    else
        printf("%c%s", ' ', "\r\n");
}

int test1_compare(test1_t * x, test1_t * y)
{
    return y->data - x->data;
}

int test2_compare(test2_t * x, test2_t * y)
{
    return y->data - x->data;
}

int count1; 
int test1_compare_reversed(test1_t * x, test1_t * y)
{
    count1++;
    return x->data - y->data;
}

int test2_compare_reversed(test2_t * x, test2_t * y)
{
    return x->data - y->data;
}

//void received_message(int id, size_t len, uint8_t * data);

void list_test(void)
{
    test1_t buf1[26];
    test1_t * head1 = NULL, * t1, ** r1;
    test2_t buf2[26];
    test2_t * head2 = NULL, * t2, ** r2;
    LL_ITERATOR it;
    //test1_t * t = NULL;
    //void * tmp;
    int i, j, tmp, cnt;
    (void)r2;

    printf("length when empty: %d\r\n", test1_length(&head1));

    for(i=0; i < 26; i++)
    {
        buf1[i].data = 'Z' - i;
        test1_push(&head1, &buf1[i]);

        buf2[i].data = 'z' - i;
        test2_push(&head2, &buf2[i]);
    }

    printf("length when full: %d\r\n", test1_length(&head1));

    test1_each(&head1, test1_print, NULL);
    test2_each(&head2, test2_print, NULL);

    //for (it = test1_iter(&head1); t1 = test1_iter_val(&it); test1_iter_next(&it))
    for_each(test1, &head1, t1, it)
    {
        if (t1 != NULL)
            printf("%c%s", t1->data, t1->next ? "->" : "\r\n");
        else
            printf("%c%s", ' ', "\r\n");
    }

    for(i=0; i < 26; i++)
    {
        t1 = test1_pop(&head1);
        test1_append(&head1, t1);
        //test1_each(&t1, test1_print, NULL);
        test1_each(&head1, test1_print, NULL);

        t2 = test2_pop(&head2);
        test2_append(&head2, t2);
        //test2_each(&t2, test2_print, NULL);
        test2_each(&head2, test2_print, NULL);
    }

    for(i=0; i < 26; i++)
    {
        t1 = test1_deduct(&head1);
        test1_push(&head1, t1);
        //test1_each(&t1, test1_print, NULL);
        test1_each(&head1, test1_print, NULL);

        t2 = test2_deduct(&head2);
        test2_push(&head2, t2);
        //test2_each(&t2, test2_print, NULL);
        test2_each(&head2, test2_print, NULL);
    }

    t1 = test1_remove(&head1, &buf1[25]);
    test1_each(&t1, test1_print, NULL);
    test1_each(&head1, test1_print, NULL);

    t2 = test2_remove(&head2, &buf2[25]);
    test2_each(&t2, test2_print, NULL);
    test2_each(&head2, test2_print, NULL);

    test1_append(&t1, test1_remove(&head1, &buf1[7]));
    test1_each(&t1, test1_print, NULL);
    test1_each(&head1, test1_print, NULL);

    test2_append(&t2, test2_remove(&head2, &buf2[7]));
    test2_each(&t2, test2_print, NULL);
    test2_each(&head2, test2_print, NULL);

    test1_append(&t1, test1_remove(&head1, &buf1[0]));
    test1_each(&t1, test1_print, NULL);
    test1_each(&head1, test1_print, NULL);

    test2_append(&t2, test2_remove(&head2, &buf2[0]));
    test2_each(&t2, test2_print, NULL);
    test2_each(&head2, test2_print, NULL);

    test1_merge(&head1, t1, test1_compare);
    test1_each(&head1, test1_print, NULL);

    test2_merge(&head2, t2, test2_compare);
    test2_each(&head2, test2_print, NULL);

    test1_sort(&head1, test1_compare_reversed);
    test1_each(&head1, test1_print, NULL);
    printf("number of comparisons: %d\r\n", count1);

    test1_sort(&head1, test1_compare);
    test1_each(&head1, test1_print, NULL);

    printf("testing merge2\r\n");

    int m2tests[] = {0x1, 0x2, 0x4, 0x8, 0x3, 0x5, 0x9, 0x11, 0x7, 0xC, 0x1B, 0x2B, 0x55};
    for(i=0; i < sizeof(m2tests)/sizeof(int); i++)
    {
        t1 = NULL;
        cnt=j=0;
        for(tmp=m2tests[i]; tmp && j < 26; tmp >>= 1)
        {
            if(tmp & 1)
            {
                test1_append(&t1, test1_remove(&head1, &buf1[25-j]));
                cnt++;
            }
            j++;
        }

        printf("remaining list: "); test1_each(&head1, test1_print, NULL);
        printf("removed list: "); test1_each(&t1, test1_print, NULL);
        r1 = test1_merge2(&t1, head1, test1_compare, cnt); //merge removed nodes with full list
        test1_each(&t1, test1_print, NULL);
        printf("merged tail: "); test1_each(r1, test1_print, NULL);
        head1 = t1;
        test1_sort(&head1, test1_compare); //restore back to before this test
        printf("\r\n");
    }

    count1 = 0;
    printf("testing sort2\r\n");
    test1_sort2(&head1, test1_compare_reversed);
    test1_each(&head1, test1_print, NULL);
    printf("number of comparisons: %d\r\n", count1);

    test1_sort2(&head1, test1_compare);
    test1_each(&head1, test1_print, NULL);

    //received_message(0, 100, (uint8_t*)"The quick Brown Fox Jumped over the Lazy Dog");
}