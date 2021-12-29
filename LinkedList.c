#include "LinkedList.h"

/* assumes variable "o" is the offset where the void * NEXT element is located */
#define NEXT(x) offsetin(x, o, void *)

/* determine the length of the linked list
   Complexity O(n)
 */
int ll_length(LL_TYPE head, const size_t o)
{
    void * x;
    int len = 0;
    /* iterate to end of list */
    for(x=*head; x; x = NEXT(x))
        len++;
    return len;
}

/* push item to the beginning of the linked list
   Complexity O(1)
 */
void ll_push(LL_TYPE head, const size_t o, void * const item)
{
    NEXT(item) = *head;
    *head = item;
}

/* pop item from the beginning of the linked list
   returns the popped item
   Complexity O(1)
 */
void * ll_pop(LL_TYPE head, const size_t o)
{
    void * x;
    x = *head;
    if(*head != NULL) {
        *head = NEXT(x);
        NEXT(x) = NULL;
    }
    return x;
}

/* append item to the end of the linked list
   Complexity O(n)
 */
void ll_append(LL_TYPE head, const size_t o, void * const item)
{
    void * x;
    if(*head == NULL) *head = item;
    else {
        /* iterate to end of list */
        for(x=*head; NEXT(x); x = NEXT(x))
            ;
        NEXT(x) = item;
    }
}

/* deduct item from the end of the linked list
   returns the deducted item
   Complexity O(n)
 */
void * ll_deduct(LL_TYPE head, const size_t o)
{
    void * x, * prev;
    if(*head == NULL) return NULL; /* empty list */
    else {
        prev = *head;
        /* iterate to end of list */
        for(x=*head; NEXT(x); x = NEXT(x))
            prev = x; /* save prev at every step */
        NEXT(prev) = NULL;
        return x;
    }
}

/* remove item from the linked list
   returns the removed item
   Complexity O(n)
 */
void * ll_remove(LL_TYPE head, const size_t o, void * const item)
{
    void * x;
    if(item == NULL || *head == NULL) return NULL;

    /* item is first in the list needs special handling */
    if(*head == item)
    {
        *head = NEXT(item); /* remove the item */
        NEXT(item) = NULL;
        return item;
    }

    /* iterate till item is found or end or list */
    for(x=*head; NEXT(x); x = NEXT(x))
    {
        if(NEXT(x) == item)
        {
            NEXT(x) = NEXT(item); /* remove the item */
            NEXT(item) = NULL;
            return item;
        }
    }
    
    /* item was not found */
    return NULL;
}

/* merge linked list "list" into head
   compare must return: 
     >= 0 if the first argument should be placed before the second
     < 0 if the first argument should be placed after the second
   Complexity O(n)
   returns pointer to last visited element of merged result
 */
static void * _ll_merge(LL_TYPE head, const size_t o, void * const list, int (*compare)(void *, void *))
{
    void * x, * y, * prev = NULL;
    
    /* sanity check*/
    if(compare == NULL || list == NULL) return *head;

    /* iterate till end of either list */
    for(x=*head, y=list; x && y;)
    {
        if(compare(x, y) >= 0)
        {
            if(prev) NEXT(prev) = x;
            else *head = x;
            prev = x;
            x = NEXT(x);
        }
        else
        {
            if(prev) NEXT(prev) = y;
            else *head = y;
            prev = y;
            y = NEXT(y);
        }
    }

    /* append remaining tail to result*/
    if(x)
    {
        if(prev) NEXT(prev) = x;
        else *head = x;
        return x;
    }
    else /*if(y), it is impossible for both x and y to be null */
    {
        if(prev) NEXT(prev) = y;
        else *head = y;
        return y;
    }
}

void ll_merge(LL_TYPE head, const size_t o, void * const list, int (*compare)(void *, void *))
{
    _ll_merge(head, o, list, compare);
}

/* sort linked list
   compare must return:
     >= 0 if the first argument should be placed before the second
     < 0 if the first argument should be placed after the second
   Complexity O(n log(n))
 */
void ll_sort(LL_TYPE head, const size_t o, int (*compare)(void *, void *))
{
    /* each merge requires sub lists which are disconnected from the 
       main list, merged, then re-attached to the main list
       To facilitate this several variables are needed to keep track
        * H: pointer to the NEXT pointer of the preceding Stub
        * M: pointer to the first element of the second list
        * T: pointer to the first element of the unused tail
       For example the list below, after splitting:
         1->2->3->4->5->6  7->8  9->10->etc.
                  H^      M^    T^
       To reconstruct, H is passed to merge, which will join
       M and H properly, then T can be appended to H.
    */
    
    void ** H, *M, *T, *x;
    int i, j, len = 0;

    /* sanity check*/
    if(compare == NULL) return;

    /* iterate once, then again for each multiple of 2 items */
    for(i=0; i == 0 || (len - 1) >> i; i++)
    {
        H = head;
        M = NULL;
        x = *H;

        for(j=1; x; j++)
        {
            if(NEXT(x) == NULL)
            { /* last merge for iteration */
                _ll_merge(H, o, M, compare);
                x = NULL;
            }
            else if(j % (1 << (i + 1)) == 0)
            { /* at end of M list, split off T then merge */
                T = NEXT(x);
                NEXT(x) = NULL;
                x = _ll_merge(H, o, M, compare);
                /* find end of merged result */
                while(NEXT(x)) x = NEXT(x);
                /* re-connect T */
                NEXT(x) = T;
                /* update H for next merge */
                H = &NEXT(x);
                M = NULL;
                x = T;
            }
            else if(j % (1 << i) == 0)
            { /* at end of H list, split off M then continue */
                M = NEXT(x);
                NEXT(x) = NULL;
                x = M;
            }
            else
            {
                /* just iterate to next node*/
                x = NEXT(x);
            }
        }

        /* first time through also calculate len*/
        if(len < j - 1) len = j - 1;
    }
}

/* merge up to n nodes from "*head" with up to n nodes from "list"
   ensures any unused nodes from list are appended to merged result
   compare must return:
     >= 0 if the first argument should be placed before the second
     < 0 if the first argument should be placed after the second
   Complexity O(n)
   returns pointer to NEXT pointer at end of merged result
 */
void ** _ll_merge2(LL_TYPE head, const size_t o, void * const list, int (*compare)(void *, void *), const int n)
{
    void * x, * y, ** prev = NULL;
    int xi, yi;

    /* sanity check*/
    if(compare == NULL) return *head;

    prev = head;
    x=*head;
    y=list;
    xi = yi = 0;
    
    /* iterate till n items from both lists */
    while(1)
    {
        /* check if list x is done */
        if(xi >= n || x == NULL)
        {
            *prev = y;
            /* iterate till end of list y */
            while(yi < n && y)
            {
                prev = &NEXT(y);
                y = NEXT(y);
                yi++;
            }
            return prev;
        }

        /* check if list y is done */
        if(yi >= n || y == NULL)
        {
            *prev = x;
            /* iterate till end of list x */
            while(xi < n && x)
            {
                prev = &NEXT(x);
                x = NEXT(x);
                xi++;
            }
            /* ensure tail of y is appended to result*/
            *prev = y;
            return prev;
        }
        
        /* compare x and y to see which is next */
        if(compare(x, y) >= 0)
        {
            *prev = x;
            prev = &NEXT(x);
            x = NEXT(x);
            xi++;
        }
        else
        {
            *prev = y;
            prev = &NEXT(y);
            y = NEXT(y);
            yi++;
        }
    }
}

/* sort linked list
   compare must return:
     >= 0 if the first argument should be placed before the second
     < 0 if the first argument should be placed after the second
   Complexity O(n log(n))
 */
void ll_sort2(LL_TYPE head, const size_t o, int (*compare)(void *, void *))
{
    /* merge2 requires the start pointer of both lists to merge
       as such the list being sorted must be iterated through
       for the start of the 2nd list, then both lists can be merged.
       Since merge2 returns the address of the pointer to the last
       visited node it isn't necessary to re-iterate the merged result
       As a result the only duplicate iteration is the first half of 
       each merge
    */

    void ** H,/* *M, *T,*/ *x;
    int i, j, len = 0;

    /* sanity check*/
    if(compare == NULL) return;

    /* iterate once, then again for each multiple of 2 items */
    for(i=0; i == 0 || (len - 1) >> i; i++)
    {
        H = head;
        x = *H ? NEXT(*H) : NULL;

        for(j=1; x; j++)
        {
            if(j % (1 << i) == 0)
            { /* at end of 1st list, merge with up to 2^i items */
                H = _ll_merge2(H, o, x, compare, 1 << i);
                j += 1 << i; /* assume other half has 2^i items */
                x = *H ? NEXT(*H) : NULL;
            }
            else
            {
                /* just iterate to next node*/
                x = NEXT(x);
            }
        }

        /* first time through also calculate len*/
        if(i == 0) len = j - 1;
    }
}

/* executes function fn on each item in the linked list
   Complexity O(n)
 */
void ll_each(LL_TYPE head, const size_t o, void (*fn)(void *, void *), void * param)
{
    void * x;
    for(x=*head; x; x = NEXT(x)) fn(x, param);
}