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

/* Extract an element of type (t) at offset (o) in the structure pointed to by (s)
 */
#define offsetin(s, o, t) *((t*)((char*)s + o))

int ll_length(void ** const head, const size_t o);
void ll_push(void ** const head, const size_t o, void * const item);
void * ll_pop(void ** const head, const size_t o);
void ll_append(void ** const head, const size_t o, void * const item);
void * ll_deduct(void ** const head, const size_t o);
void * ll_remove(void ** const head, const size_t o, void * const item);
void ll_merge(void ** const head, const size_t o, void * const list, int (*compare)(void *, void *));
void ll_sort(void ** const head, const size_t o, int (*compare)(void *, void *));
void ll_each(void ** const head, const size_t o, void (*fn)(void **, void *), void * param);
void ** _ll_merge2(void ** const head, const size_t o, void * const list, int (*compare)(void *, void *), const int n);
void ll_sort2(void ** const head, const size_t o, int (*compare)(void *, void *));

#endif // !__LINKED_LIST_H__

#if defined(TEMPLATE_PREFIX) && defined(TEMPLATE_LINKED_LIST_STRUCT) && defined(TEMPLATE_LINKED_LIST_NEXT)

/*shorter versions of the template definitions*/
#define PREFIX PPCAT(TEMPLATE_PREFIX, _)
#define STRUCT TEMPLATE_LINKED_LIST_STRUCT
#define OFFSET offsetof(STRUCT, next)

#define FUNCTION(name) PPCAT(PREFIX, name)

/* determine the length of the linked list
   Complexity O(n)
 */
static inline int FUNCTION(length)(STRUCT ** const head)
{
    return ll_length(head, OFFSET);
}

/* push item to the beginning of the linked list
   Complexity O(1)
 */
static inline void FUNCTION(push)(STRUCT ** const head, STRUCT * const item)
{
    ll_push(head, OFFSET, item);
}

/* pop item from the beginning of the linked list
   returns the popped item
   Complexity O(1)
 */
static inline STRUCT * FUNCTION(pop)(STRUCT ** const head)
{
    return ll_pop(head, OFFSET);
}

/* append item to the end of the linked list
   Complexity O(n)
 */
static inline void FUNCTION(append)(STRUCT ** const head, STRUCT * const item)
{
    ll_append(head, OFFSET, item);
}

/* deduct item from the end of the linked list
   returns the deducted item
   Complexity O(n)
 */
static inline STRUCT * FUNCTION(deduct)(STRUCT ** const head)
{
    return ll_deduct(head, OFFSET);
}

/* remove item from the linked list
   returns the removed item
   Complexity O(n)
 */
static inline STRUCT * FUNCTION(remove)(STRUCT ** const head, STRUCT * const item)
{
    return ll_remove(head, OFFSET, item);
}

/* merge linked list "list" into head
   compare must return:
     >= 0 if the first argument should be placed before the second
     < 0 if the first argument should be placed after the second
   Complexity O(n)
 */
static inline void FUNCTION(merge)(STRUCT ** const head, STRUCT * const list, int (*compare)(STRUCT *, STRUCT *))
{
    ll_merge(head, OFFSET, list, compare);
}

/* sort linked list
   compare must return:
     >= 0 if the first argument should be placed before the second
     < 0 if the first argument should be placed after the second
   Complexity O(n log(n))
 */
void FUNCTION(sort)(STRUCT ** const head, int (*compare)(STRUCT *, STRUCT *))
{
    ll_sort(head, OFFSET, compare);
}

/* merge up to n nodes from "*head" with up to n nodes from "list"
   ensures any unused nodes from list are appended to merged result
   compare must return:
     >= 0 if the first argument should be placed before the second
     < 0 if the first argument should be placed after the second
   Complexity O(n)
   returns pointer to NEXT pointer at end of merged result
 */
static inline STRUCT ** FUNCTION(merge2)(STRUCT ** const head, STRUCT * const list, int (*compare)(STRUCT *, STRUCT *), const int n)
{
    return (STRUCT **)_ll_merge2(head, OFFSET, list, compare, n);
}

/* sort linked list
   compare must return:
     >= 0 if the first argument should be placed before the second
     < 0 if the first argument should be placed after the second
   Complexity O(n log(n))
 */
void FUNCTION(sort2)(STRUCT ** const head, int (*compare)(STRUCT *, STRUCT *))
{
    ll_sort2(head, OFFSET, compare);
}

static inline void FUNCTION(each)(STRUCT ** const head, void (*fn)(STRUCT *, void *), void * const param)
{
    /* The below cast is technically undefined behavior...
     * let me know if you find a system it fails in */
    ll_each(head, OFFSET, (void (*)(void *, void *))fn, param);
}

/* un-define all the template magic */
#undef PREFIX
#undef STRUCT
#undef OFFSET

#undef TEMPLATE_PREFIX 
#undef TEMPLATE_LINKED_LIST_STRUCT
#undef TEMPLATE_LINKED_LIST_NEXT

#undef FUNCTION

#endif // TEMPLATE
