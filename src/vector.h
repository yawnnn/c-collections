#ifndef __VECTOR_H__
#define __VECTOR_H__

#include "gendef.h"

typedef struct vec {
    char *ptr;
    size_t cap;
    size_t len;
    size_t size;
} Vec;

#define VEC_SORT_ASC     1  
#define VEC_SORT_DESC   -1

void vec_new(Vec *v, size_t size);
void vec_new_with(Vec *v, size_t size, size_t cap);
void vec_new_from(Vec *v, size_t size, void *arr, size_t n);
void vec_init(Vec *v, size_t size, size_t n);
void vec_clear(Vec *v);
void vec_reserve(Vec *v, size_t n);
void vec_shrink_to_fit(Vec *v);
void *vec_data(Vec *v);
void *vec_at(Vec *v, size_t pos);
void vec_push(Vec *v, void *elem);
void vec_insert(Vec *v, void *elem, size_t pos);
void vec_insert_n(Vec *v, void *elems, size_t pos, size_t n);
void vec_merge(Vec *v1, Vec *v2) ;
void vec_pop(Vec *v, void *elem);
void vec_remove(Vec *v, size_t pos, void *elem);
void vec_remove_n(Vec *v, size_t pos, size_t n, void *elem);
void vec_get(Vec *v, size_t pos, void *elem);
void vec_set(Vec *v, void *elem, size_t pos);
size_t vec_len(Vec *v);
size_t vec_capacity(Vec *v);
size_t vec_max_size();
size_t vec_sizeof(Vec *v);
bool vec_empty(Vec *v);
void vec_swap(Vec *v, size_t pos1, size_t pos2, void *tmp);
void vec_sort(Vec *v, int order);
bool vec_iter(Vec *v, void *elem);

#define vec_iter_reset()    vec_iter(NULL, NULL)

#endif  /* __VECTOR_H__ */