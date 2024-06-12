#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

#include <stddef.h>

/* Concatenate preprocessor tokens A and B without expanding macro definitions
   (however, if invoked from a macro, macro arguments are expanded).
 */
#define PPCAT_NX(A, B) A ## B

/* Concatenate preprocessor tokens A and B after macro-expanding them.
 */
#define PPCAT(A, B) PPCAT_NX(A, B)

/* Extract an element of type (member_type) at offset (offset) in the structure pointed to by (ptr)
 */
#define offsetin(ptr, offset, member_type) *((member_type*)((char*)ptr + offset))

typedef void ** LL_TYPE;
typedef int (*LL_COMPARE)(void *, void *);
typedef struct {
    void * n;
} LL_ITERATOR;

int ll_length(LL_TYPE head, size_t o);
void ll_push(LL_TYPE head, size_t o, void * item);
void * ll_pop(LL_TYPE head, size_t o);
void ll_append(LL_TYPE head, size_t o, void * item);
void * ll_deduct(LL_TYPE head, size_t o);
void * ll_remove(LL_TYPE head, size_t o, void * item);
void * ll_find(const LL_TYPE head, size_t o, void * item, LL_COMPARE);
void ll_merge(LL_TYPE head, size_t o, void * list, LL_COMPARE);
void ll_sort(LL_TYPE head, size_t o, LL_COMPARE);
void ll_each(const LL_TYPE head, size_t o, void (*fn)(void *, void *), void * param);
void ** _ll_merge2(LL_TYPE head, size_t o, void * list, LL_COMPARE, int n);
void ll_sort2(LL_TYPE head, size_t o, LL_COMPARE);

LL_ITERATOR ll_iter(const LL_TYPE head);
void * ll_iter_val(LL_ITERATOR* it);
void ll_iter_next(LL_ITERATOR* it, size_t o);

#endif // !__LINKED_LIST_H__

#if defined(TEMPLATE_PREFIX) && defined(TEMPLATE_STRUCT) && defined(TEMPLATE_STRUCT)

/*shorter versions of the template definitions*/
#define PREFIX PPCAT(TEMPLATE_PREFIX, _)
#define STRUCT TEMPLATE_STRUCT
#define OFFSET offsetof(STRUCT, next)

#define FUNCTION(name) PPCAT(PREFIX, name)

/* determine the length of the linked list
   Complexity O(n)
 */
static inline int FUNCTION(length)(STRUCT ** head)
{
    return ll_length((LL_TYPE)head, OFFSET);
}

/* push item to the beginning of the linked list
   Complexity O(1)
 */
static inline void FUNCTION(push)(STRUCT ** head, STRUCT * item)
{
    ll_push((LL_TYPE)head, OFFSET, item);
}

/* pop item from the beginning of the linked list
   returns the popped item
   Complexity O(1)
 */
static inline STRUCT * FUNCTION(pop)(STRUCT ** head)
{
    return (STRUCT *)ll_pop((LL_TYPE)head, OFFSET);
}

/* append item to the end of the linked list
   Complexity O(n)
 */
static inline void FUNCTION(append)(STRUCT ** head, STRUCT * item)
{
    ll_append((LL_TYPE)head, OFFSET, item);
}

/* deduct item from the end of the linked list
   returns the deducted item
   Complexity O(n)
 */
static inline STRUCT * FUNCTION(deduct)(STRUCT ** head)
{
    return (STRUCT *)ll_deduct((LL_TYPE)head, OFFSET);
}

/* remove item from the linked list
   returns the removed item
   Complexity O(n)
 */
static inline STRUCT * FUNCTION(remove)(STRUCT ** head, STRUCT * item)
{
    return (STRUCT *)ll_remove((LL_TYPE)head, OFFSET, item);
}

/* find a match to item in the linked list
   compare must return:
     == 0 if this is the desired item in the list
   item will be passed to compare as the second argument
   Complexity O(n)
 */
static inline STRUCT * FUNCTION(find)(STRUCT ** head, void * item, int (*compare)(STRUCT *, void *))
{
    return (STRUCT *)ll_find((LL_TYPE)head, OFFSET, item, (LL_COMPARE)compare);
}

/* merge linked list "list" into head
   compare must return:
     >= 0 if the first argument should be placed before the second
     < 0 if the first argument should be placed after the second
   Complexity O(n)
 */
static inline void FUNCTION(merge)(STRUCT ** head, STRUCT * list, int (*compare)(STRUCT *, STRUCT *))
{
    ll_merge((LL_TYPE)head, OFFSET, list, (LL_COMPARE)compare);
}

/* sort linked list
   compare must return:
     >= 0 if the first argument should be placed before the second
     < 0 if the first argument should be placed after the second
   Complexity O(n log(n))
 */
static inline void FUNCTION(sort)(STRUCT ** head, int (*compare)(STRUCT *, STRUCT *))
{
    ll_sort((LL_TYPE)head, OFFSET, (LL_COMPARE)compare);
}

/* merge up to n nodes from "*head" with up to n nodes from "list"
   ensures any unused nodes from list are appended to merged result
   compare must return:
     >= 0 if the first argument should be placed before the second
     < 0 if the first argument should be placed after the second
   Complexity O(n)
   returns pointer to NEXT pointer at end of merged result
 */
static inline STRUCT ** FUNCTION(merge2)(STRUCT ** head, STRUCT * list, int (*compare)(STRUCT *, STRUCT *), int n)
{
    return (STRUCT **)_ll_merge2((LL_TYPE)head, OFFSET, list, (LL_COMPARE)compare, n);
}

/* sort linked list
   compare must return:
     >= 0 if the first argument should be placed before the second
     < 0 if the first argument should be placed after the second
   Complexity O(n log(n))
 */
static inline void FUNCTION(sort2)(STRUCT ** head, int (*compare)(STRUCT *, STRUCT *))
{
    ll_sort2((LL_TYPE)head, OFFSET, (LL_COMPARE)compare);
}

static inline void FUNCTION(each)(STRUCT ** head, void (*fn)(STRUCT *, void *), void * param)
{
    /* The below cast is technically undefined behavior...
     * let me know if you find a system it fails in */
    ll_each((LL_TYPE)head, OFFSET, (void (*)(void *, void *))fn, param);
}

static inline LL_ITERATOR FUNCTION(iter)(STRUCT** head)
{
    return ll_iter((LL_TYPE)head);
}

static inline STRUCT * FUNCTION(iter_val)(LL_ITERATOR* it)
{
    return ll_iter_val(it);
}

static inline void FUNCTION(iter_next)(LL_ITERATOR* it)
{
    ll_iter_next(it, OFFSET);
}

#ifndef for_each
/* Shorthand for:
 * for (i = PREFIX_iter(ll); v = PREFIX_iter_val(&i); PREFIX_iter_next(&i)) */
#define for_each(pre, ll, v, i) for \
    (i = PPCAT(PPCAT(pre, _), iter)(ll);\
    v = PPCAT(PPCAT(pre, _), iter_val)(&i);\
    PPCAT(PPCAT(pre, _), iter_next)(&i))
#endif // !for_each

/* un-define all the template magic */
#undef PREFIX
#undef STRUCT
#undef OFFSET

#undef TEMPLATE_PREFIX 
#undef TEMPLATE_STRUCT
#undef TEMPLATE_STRUCT

#undef FUNCTION

#endif // TEMPLATE
