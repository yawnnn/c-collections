#include "vector.h"

#define GROWTH_FACTOR   (2UL)

/* ======================================================================================== */
/*                                           DEBUG                                          */
/* ======================================================================================== */

#if __DBG__
#define __VEC_DEBUG_MODE 1
#else
#define __VEC_DEBUG_MODE 0
#endif

static const char max_depth_tabs[20] = "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t";
static short int __dbg_depth = 0;

static void vec_dbg(Vec *v)
{
    printf("Vec => ptr: " CCYAN "%p" CRESET ", cap: %u, len: %u, size: %u\n", v->ptr, v->cap, v->len, v->size);
}

#define __DBG_PRINT_BEFORE(v)                                   \
if (__VEC_DEBUG_MODE) {                                         \
    printf("%.*s", __dbg_depth++, max_depth_tabs);              \
    printf(CRED "DBG(" CGREEN "before" CRESET CRED ")" CRESET ": In " CYELLOW "%s" CRESET "(). ", __func__); \
    vec_dbg(v);                                                 \
}                                                               \

#define __DBG_PRINT_AFTER(v)                                    \
if (__VEC_DEBUG_MODE) {                                         \
    printf("%.*s", --__dbg_depth, max_depth_tabs);              \
    printf(CRED "DBG(" CMAGENTA "after" CRESET CRED ")" CRESET ": In " CYELLOW "%s" CRESET "(). ", __func__); \
    vec_dbg(v);                                                 \
}    

/* ======================================================================================== */
/*                                     PRIVATE METHODS                                      */
/* ======================================================================================== */

inline static char *v_at(Vec *v, uint pos)
{
    return v->ptr + (pos * v->size);
}

inline static void v_set(Vec *v, uint pos, uint n)
{
    memset(v_at(v, pos), 0, n * v->size);
}

inline static void v_cpy(Vec *v, uint pos, void *source, uint n)
{
    memcpy(v_at(v, pos), source, n * v->size);
}

inline static void v_cpy_to(void *dest, Vec *v, uint pos, uint n)
{
    memcpy(dest, v_at(v, pos), n * v->size);
}

inline static void v_move(Vec *v, uint pos, void *source, uint n)
{
    memmove(v_at(v, pos), source, n * v->size);
}

inline static int v_cmp(Vec *v, uint pos, void *ptr2, uint n)
{
    return memcmp(v_at(v, pos), ptr2, n * v->size);
}

inline static void v_alloc(Vec *v, uint n)
{
    v->ptr = malloc(n * v->size);
    v->cap = n;
}

inline static void v_realloc(Vec *v, uint n)
{
    v->ptr = realloc(v->ptr, n * v->size);
    v->cap = n;
}

/* Increase capacity by GROWTH_FACTOR */
static void v_grow(Vec *v) 
{
    __DBG_PRINT_BEFORE(v);
    if (v->cap == 0)
        v_alloc(v, 1);
    else
        v_realloc(v, v->cap * GROWTH_FACTOR);
    __DBG_PRINT_AFTER(v);
}

/* Grow where possible, otherwise realloc */
static void v_resize(Vec *v, uint n) 
{
    __DBG_PRINT_BEFORE(v);
    if (v->cap) {
        if (n < v->cap || n > v->cap * GROWTH_FACTOR)
            v_realloc(v, n);
        else if (n > v->cap)
            v_grow(v);
    }
    else {
        v_alloc(v, n);
    }
    __DBG_PRINT_AFTER(v);
}

/* ======================================================================================== */
/*                                      PUBLIC METHODS                                      */
/* ======================================================================================== */

/* Sets the variables for vector with elements of <size>. The vector is still NULL */
void vec_new(Vec *v, uint size) 
{
    v->ptr = NULL;
    v->cap = 0;
    v->len = 0;
    v->size = size;
}

/* New vector of elements of <size>, with capacity <cap>
 * the elements are not initialized and length is 0 */
void vec_new_with(Vec *v, uint cap, uint size)
{
    vec_new(v, size);
    vec_reserve(v, cap);
}

/* New vector with <n> elements of <size>. everything memset to 0 */
void vec_init(Vec *v, uint n, uint size)
{
    vec_new_with(v, n, size);
    v_set(v, 0, n);
    v->len = n;
}

/* Create vector from a c-style array <arr> with <n> elements of <size> */
void vec_new_from(Vec *v, void *arr, uint n, uint size)
{
    vec_new_with(v, n, size);
    vec_insert_n(v, arr, 0, n);
}

/* Clear variables, free memory for the array. 
 * If necessary, the single elements need to be freed by the user before this */
void vec_clear(Vec *v) 
{
    if (v->cap)
        free(v->ptr);
    v->cap = 0;
    v->len = 0;
}

/* Reserve memory for at least <n> number of elements */
void vec_reserve(Vec *v, uint n)
{
    if (n > v->cap)
        v_resize(v, n);
}

/* Ensures the memory allocated is exactly as needed for the length */
void vec_shrink_to_fit(Vec *v)
{
    if (v->cap)
        v_resize(v, v->len);
}

/* Returns the underlying pointer. 
 * If changes to the vector are made, this pointer can become invalid */
void *vec_data(Vec *v)
{
    if (v->len)
        return (void *)v->ptr;
    return NULL;
}

/* Pointer to element at <pos>. 
 * If changes to the vector are made, this pointer can become invalid */
void *vec_at(Vec *v, uint pos)
{
    if (pos < v->len)
        return v_at(v, pos);
    return NULL;
}

/* Insert <elem> at the end of the vector */
void vec_push(Vec *v, void *elem) 
{
    __DBG_PRINT_BEFORE(v);
    vec_insert(v, elem, v->len);
    __DBG_PRINT_AFTER(v);
}

/* Insert <elem> at <pos> */
void vec_insert(Vec *v, void *elem, uint pos)
{
    __DBG_PRINT_BEFORE(v);
    if (pos <= v->len) {
        vec_reserve(v, v->len + 1);
        v_move(v, pos + 1, v_at(v, pos), v->len - pos);
        v_cpy(v, pos, elem, 1);
        v->len++;
    }
    __DBG_PRINT_AFTER(v);
}

/* Insert <n> <elems> starting from <pos> */
void vec_insert_n(Vec *v, void *elems, uint pos, uint n)
{
    __DBG_PRINT_BEFORE(v);
    if (pos <= v->len) {
        vec_reserve(v, v->len + n);
        v_move(v, pos + n, v_at(v, pos), v->len - pos);
        v_cpy(v, pos, elems, n);
        v->len += n;
    }
    __DBG_PRINT_AFTER(v);
}

/* Remove element from the end of the array
 * If <elem> != NULL the element is copied to it, so that memory it owns can be freed */
void vec_pop(Vec *v, void *elem) 
{
    if (v->len) {
        if (elem)
            v_cpy_to(elem, v, v->len - 1, 1);
        v->len--;

        // Should i shrink?
        //if (v->len <= (v->cap / GROWTH_FACTOR))
        //    vec_shrink_to_fit(v);
    }
}

/* Remove the element at <pos>. 
 * If <elem> != NULL the element is copied to it, so that memory it owns can be freed */
void vec_remove(Vec *v, uint pos, void *elem) 
{
    __DBG_PRINT_BEFORE(v);
    if (pos < v->len) {
        if (elem)
            v_cpy_to(elem, v, pos, 1);
        if (pos + 1 < v->len)
            v_move(v, pos, v_at(v, pos + 1), v->len - (pos + 1));
        vec_pop(v, NULL);
    }
    __DBG_PRINT_AFTER(v);
}

/* memcpy of element at <pos> to <elem> */
void vec_get(Vec *v, uint pos, void *elem)
{
    if (pos < v->len)
        v_cpy_to(elem, v, pos, 1);
}

/* memcpy of <elem> to element at <pos> */
void vec_set(Vec *v, void *elem, uint pos) 
{
    if (pos < v->len)
        v_cpy(v, pos, elem, 1);
}

/* Length of vector, number of accessible elements */
uint vec_len(Vec *v) 
{
    return v->len;
}

/* Capacity of vector, the memory allocated for it */
uint vec_capacity(Vec *v)
{
    return v->cap;
}

/* The maximum size of elements a vector can hold */
uint vec_max_size()
{
    return (uint)-1;
}

/* The size of the elements cointained in vector */
uint vec_sizeof(Vec *v)
{
    return v->size;
}

/* If the vector holds elements */
bool vec_empty(Vec *v)
{
    return v->len != 0;
}

/* Swap elements at pos <pos1> and <pos2>
 * For simplicity, a pointer to a struct element <tmp> is required */
void vec_swap(Vec *v, uint pos1, uint pos2, void *tmp)
{
    v_cpy_to(tmp, v, pos1, 1);
    v_cpy(v, pos1, v_at(v, pos2), 1);
    v_cpy(v, pos2, tmp, 1);
}

/* Sort in <order> VEC_SORT_ASC or VEC_SORT_DESC */
/* TODO --- Better algorithm */
void vec_sort(Vec *v, int order)
{
    int i, j;
    char val[32];
    void *pval;
    bool alloc;

    if (v->size < sizeof(val)) {
        pval = val;
        alloc = NO;
    }
    else {
        pval = malloc(v->size);
        alloc = YES;
    }

    for (i = 0; i < v->len; i++) {
        for (j = i + 1; j < v->len; j++) {
            if (v_cmp(v, i, v_at(v, j), 1) * order > 0)
                vec_swap(v, i, j, pval);
        }
    }

    if (alloc)
        free(pval);
}

/* Iterate over elements of array.
 * Always call first with (NULL, NULL), to reset the interal counter
 * Returns wether there was another element 
 * Elements are read in <elem> */
bool vec_iter(Vec *v, void *elem) 
{
    static uint i = 0;

    if (v) {
        if (i < v->len) {
            vec_get(v, i++, elem);
            return YES;
        }
    } else {
        i = 0;
    }

    return NO;
}