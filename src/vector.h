#ifndef __VECTOR_H__
#define __VECTOR_H__

#include "gendef.h"

typedef struct vec {
    char *ptr;
    uint cap;
    uint len;
    uint size;
} Vec;

#define VEC_SORT_ASC     1  
#define VEC_SORT_DESC   -1

typedef char*(*PFN_TO_STRING)(void *, char *);

void vec_new(Vec *v, uint size);
void vec_new_with(Vec *v, uint cap, uint size);
void vec_new_from(Vec *v, void *arr, uint n, uint size);
void vec_init(Vec *v, uint n, uint size);
void vec_clear(Vec *v);
void vec_reserve(Vec *v, uint n);
void vec_shrink_to_fit(Vec *v);
void *vec_data(Vec *v);
void *vec_at(Vec *v, uint pos);
void vec_push(Vec *v, void *elem);
void vec_insert(Vec *v, void *elem, uint pos);
void vec_insert_n(Vec *v, void *elems, uint pos, uint n);
void vec_merge(Vec *v1, Vec *v2) ;
void vec_pop(Vec *v, void *elem);
void vec_remove(Vec *v, uint pos, void *elem);
void vec_get(Vec *v, uint pos, void *elem);
void vec_set(Vec *v, void *elem, uint pos);
uint vec_len(Vec *v);
uint vec_capacity(Vec *v);
uint vec_max_size();
uint vec_sizeof(Vec *v);
bool vec_empty(Vec *v);
void vec_swap(Vec *v, uint pos1, uint pos2, void *tmp);
void vec_sort(Vec *v, int order);
bool vec_iter(Vec *v, void *elem);
void vec_print(Vec *v, PFN_TO_STRING to_string, char *separator);

#define vec_first(v)        (0)                 /* index of first element of vector */
#define vec_last(v)         (vec_len(v) - 1)    /* index of last element of vector */
#define vec_end(v)          (vec_len(v))        /* end of the vector, not a valid position to access */
#define vec_iter_reset()    vec_iter(NULL, NULL)

#endif  /* __VECTOR_H__ */