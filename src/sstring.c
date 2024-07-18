#include "sstring.h"

#include <stdlib.h>
#include <string.h>

#define GROWTH_FACTOR (2UL)

/********************************************************************************************
 *                                     PRIVATE METHODS                                      *
 ********************************************************************************************/

inline static void s_alloc(SStr *s, size_t nbytes) {
    s->ptr = malloc(nbytes);
    s->cap = nbytes;
}

inline static void s_realloc(SStr *s, size_t nbytes) {
    s->ptr = realloc(s->ptr, nbytes);
    s->cap = nbytes;
}

/**
 * @brief resize SStr.
 * 
 * if shrink, realloc by exact number
 * if grow  , realloc by GROWTH_FACTOR when possible, otherwise exact number
 * 
 * @param s SStr
 * @param nbytes number of bytes required
 */
static void sstr_resize(SStr *s, size_t nbytes) {
    if (s->cap) {
        if (nbytes < s->cap || nbytes > s->cap * GROWTH_FACTOR)
            s_realloc(s, nbytes);
        else if (nbytes > s->cap)
            s_realloc(s, s->cap * GROWTH_FACTOR);
    } else {
        s_alloc(s, nbytes > GROWTH_FACTOR ? nbytes : GROWTH_FACTOR);
    }
}

/********************************************************************************************
 *                                      PUBLIC METHODS                                      *
 ********************************************************************************************/

void sstr_new(SStr *s) {
    s->cap = 0;
    s->len = 0;
}

void sstr_new_with(SStr *s, size_t len) {
    sstr_new(s);
    sstr_reserve(s, len);
    sstr_cpy(s, "");
}

void sstr_from(SStr *s, const char *source) {
    sstr_new(s);
    sstr_cpy(s, source);
}

void sstr_free(SStr *s) {
    if (s->cap)
        free(s->ptr);
    s->cap = 0;
    s->len = 0;
}

void sstr_reserve(SStr *s, size_t len) {
    if (len + 1 > s->cap)
        sstr_resize(s, len + 1);
}

void sstr_shrink_to_fit(SStr *s) {
    if (s->cap > s->len + 1)
        sstr_resize(s, s->len + 1);
}

char *sstr_cpy(SStr *dest, const char *source) {
    dest->len = strlen(source);
    sstr_reserve(dest, dest->len);
    return strcpy(dest->ptr, source);
}

char *sstr_ncpy(SStr *dest, const char *source, size_t num) {
    dest->len = strlen(source);
    if (dest->len > num)
        dest->len = num;
    sstr_reserve(dest, dest->len);
    dest->ptr[dest->len] = '\0';
    return strncpy(dest->ptr, source, dest->len);
}

char *sstr_cat(SStr *dest, const char *source) {
    dest->len += strlen(source);
    sstr_reserve(dest, dest->len);
    return strcat(dest->ptr, source);
}

char *sstr_ncat(SStr *dest, const char *source, size_t num) {
    size_t len_src;

    len_src = strlen(source);
    if (len_src < num)
        num = len_src;
    dest->len += num;
    sstr_reserve(dest, dest->len);
    dest->ptr[dest->len] = '\0';
    return strncat(dest->ptr, source, num);
}

char *sstr_merge(SStr *dest, SStr *source, const char *sep) {
    if (source->len) {
        dest->len += source->len + strlen(sep);
        sstr_reserve(dest, dest->len);
        strcat(strcat(dest->ptr, sep), source->ptr);
    }
    sstr_free(source);
    return dest->ptr;
}