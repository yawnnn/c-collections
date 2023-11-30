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
    printf("Vec => ptr: " CCYAN "%p" CRESET ", cap: %zu, len: %zu, size: %zu\n", v->ptr, v->cap, v->len, v->size);
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

inline static char *v_at(Vec *v, size_t pos)
{
    return v->ptr + (pos * v->size);
}

inline static void v_set(Vec *v, size_t pos, size_t n)
{
    memset(v_at(v, pos), 0, n * v->size);
}

inline static void v_cpy(Vec *v, size_t pos, void *source, size_t n)
{
    memcpy(v_at(v, pos), source, n * v->size);
}

inline static void v_cpy_to(void *dest, Vec *v, size_t pos, size_t n)
{
    memcpy(dest, v_at(v, pos), n * v->size);
}

inline static void v_move(Vec *v, size_t pos, void *source, size_t n)
{
    memmove(v_at(v, pos), source, n * v->size);
}

inline static int v_cmp(Vec *v, size_t pos, void *ptr2, size_t n)
{
    return memcmp(v_at(v, pos), ptr2, n * v->size);
}

inline static void v_alloc(Vec *v, size_t n)
{
    v->ptr = malloc(n * v->size);
    v->cap = n;
}

inline static void v_realloc(Vec *v, size_t n)
{
    v->ptr = realloc(v->ptr, n * v->size);
    v->cap = n;
}

/* increase capacity by GROWTH_FACTOR */
static void v_grow(Vec *v) 
{
    __DBG_PRINT_BEFORE(v);
    if (v->cap == 0)
        v_alloc(v, GROWTH_FACTOR);
    else
        v_realloc(v, v->cap * GROWTH_FACTOR);
    __DBG_PRINT_AFTER(v);
}

/* grow where possible, otherwise realloc */
static void v_resize(Vec *v, size_t n) 
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

/* sets the variables for vector with elements of <size>. The vector is still NULL */
void vec_new(Vec *v, size_t size) 
{
    v->cap = 0;
    v->len = 0;
    v->size = size;
}

/* new vector of elements of <size>, with capacity <n>
 * the elements are not initialized and length is 0 */
void vec_new_with(Vec *v, size_t size, size_t n)
{
    vec_new(v, size);
    vec_reserve(v, n);
}

/* new vector with <n> elements of <size>. everything memset to 0 */
void vec_init(Vec *v, size_t size, size_t n)
{
    vec_new_with(v, n, size);
    v_set(v, 0, n);
    v->len = n;
}

/* new vector from a c-style array <arr> with <n> elements of <size> */
void vec_from(Vec *v, size_t size, void *arr, size_t n)
{
    vec_new(v, size);
    vec_insert_n(v, arr, 0, n);
}

/* clear variables, release memory. 
 * if necessary, the single elements need to be freed by the user before this */
void vec_clear(Vec *v) 
{
    if (v->cap)
        free(v->ptr);
    v->cap = 0;
    v->len = 0;
}

/* reserve memory for at least <n> number of elements */
void vec_reserve(Vec *v, size_t n)
{
    if (n > v->cap)
        v_resize(v, n);
}

/* ensures the memory allocated is exactly as needed for the length */
void vec_shrink_to_fit(Vec *v)
{
    if (v->cap)
        v_resize(v, v->len);
}

/* insert <elem> at the end of the vector */
void vec_push(Vec *v, void *elem) 
{
    __DBG_PRINT_BEFORE(v);
    vec_insert(v, elem, v->len);
    __DBG_PRINT_AFTER(v);
}

/* insert <elem> at <pos> */
void vec_insert(Vec *v, void *elem, size_t pos)
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

/* insert <n> <elems> starting from <pos> */
void vec_insert_n(Vec *v, void *elems, size_t n, size_t pos)
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

/* remove element from the end of the array
 * if <elem> != NULL the element is copied to it, so that memory it owns can be freed by the caller */
void vec_pop(Vec *v, void *elem) 
{
    if (v->len) {
        if (elem)
            v_cpy_to(elem, v, v->len - 1, 1);
        v->len--;
    }
}

/* remove the element at <pos>. 
 * if <elem> != NULL the element is copied to it, so that memory it owns can be freed by the caller */
void vec_remove(Vec *v, size_t pos, void *elem) 
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

/* remove the <n> elements starting at <pos>. 
 * if <elems> != NULL the elements are copied to it, so that memory they own can be freed by the caller */
void vec_remove_n(Vec *v, size_t pos, size_t n, void *elems)
{
    __DBG_PRINT_BEFORE(v);
    if (pos + n - 1 < v->len) {
        if (elems)
            v_cpy_to(elems, v, pos, n);
        if (pos + n < v->len)
            v_move(v, pos, v_at(v, pos + n), v->len - (pos + n));
        v->len -= n;
    }
    __DBG_PRINT_AFTER(v);
}

/* return element at <pos> in <elem> */
void vec_get(Vec *v, size_t pos, void *elem)
{
    if (pos < v->len)
        v_cpy_to(elem, v, pos, 1);
}

/* set element at <pos> equal to <elem> */
void vec_set(Vec *v, void *elem, size_t pos) 
{
    if (pos < v->len)
        v_cpy(v, pos, elem, 1);
}

/* returns the underlying pointer. 
 * if changes to the vector are made, this pointer can become invalid */
inline void *vec_data(Vec *v)
{
    if (v->len)
        return (void *)v->ptr;
    return NULL;
}

/* pointer to element at <pos>. 
 * if changes to the vector are made, this pointer can become invalid */
inline void *vec_at(Vec *v, size_t pos)
{
    if (pos < v->len)
        return v_at(v, pos);
    return NULL;
}

/* length of vector, number of accessible elements */
inline size_t vec_len(Vec *v) 
{
    return v->len;
}

/* capacity of vector, the number of elements it is allocated for (not bytes) */
inline size_t vec_capacity(Vec *v)
{
    return v->cap;
}

/* the maximum number of elements a vector with this size of element will ever hold */
inline size_t vec_max_size(Vec *v)
{
    return ((size_t)-1) / v->size;
}

/* the size of the elements cointained in vector */
inline size_t vec_sizeof(Vec *v)
{
    return v->size;
}

/* if the vector is empty */
inline bool vec_empty(Vec *v)
{
    return v->len == 0;
}

/* swap elements at pos <pos1> and <pos2>
 * for simplicity, a pointer to a element <tmp> is required */
void vec_swap(Vec *v, size_t pos1, size_t pos2, void *tmp)
{
    v_cpy_to(tmp, v, pos1, 1);
    v_cpy(v, pos1, v_at(v, pos2), 1);
    v_cpy(v, pos2, tmp, 1);
}

/* sort in <order> [VEC_SORT_ASC|VEC_SORT_DESC] */
/* TODO --- Better algorithm */
void vec_sort(Vec *v, int order)
{
    int i, j;
    char val[256];
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

/* iterate over elements of array.
 * call first with vec_iter_reset() to reset the interal counter
 * read element in <elem>, returns NO if it's done */
bool vec_iter(Vec *v, void *elem) 
{
    static size_t i = 0;

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