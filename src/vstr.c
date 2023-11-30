#include "gendef.h"
#include "vstr.h"

/* ================== BEHAVIOR ================== */

#define GROWTH_FACTOR   (2UL)

/* ================== DEBUG ================== */

#if __DBG__
#define __STR_DEBUG_MODE 1
#else
#define __STR_DEBUG_MODE 0
#endif

static const char max_depth_tabs[20] = "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t";
static short int __dbg_depth = 0;

static void vstr_dbg(Vstr *s)
{
    printf("Vstr => s: %s, cap: %zu, len: %zu, strlen: %d\n", s->ptr, s->cap, s->len, s->ptr && s->cap ? (int)strlen(s->ptr) : (int)-1);
}

#define __DBG_PRINT_BEFORE(s)                                    \
if (__STR_DEBUG_MODE) {                                          \
    printf("%.*s", __dbg_depth++, max_depth_tabs);               \
    printf(CRED "DBG(" CGREEN "before" CRESET CRED ")" CRESET ": In " CYELLOW "%s" CRESET "(). ", __func__); \
    vstr_dbg(s);                                                 \
}                                                                \

#define __DBG_PRINT_AFTER(s)                                     \
if (__STR_DEBUG_MODE) {                                          \
    printf("%.*s", --__dbg_depth, max_depth_tabs);               \
    printf(CRED "DBG(" CMAGENTA "after" CRESET CRED ")" CRESET ": In " CYELLOW "%s" CRESET "(). ", __func__); \
    vstr_dbg(s);                                                 \
}    

/* ======================================================================================== */
/*                                     PRIVATE METHODS                                      */
/* ======================================================================================== */

inline static void vs_alloc(Vstr *s, size_t n)
{
    s->ptr = malloc(n);
    s->cap = n;
}

inline static void vs_realloc(Vstr *s, size_t n)
{
    s->ptr = realloc(s->ptr, n);
    s->cap = n;
}

/* if shrink, realloc by exact number
 * if grow  , realloc by GROWTH_FACTOR when possible, otherwise exact number */
static void vs_resize(Vstr *s, size_t n) 
{
    if (s->cap) {
        if (n < s->cap || n > s->cap * GROWTH_FACTOR)
            vs_realloc(s, n);
        else if (n > s->cap)
            vs_realloc(s, s->cap * GROWTH_FACTOR);
    } else {
        vs_alloc(s, n > GROWTH_FACTOR ? n : GROWTH_FACTOR);
    }
}

/* ======================================================================================== */
/*                                      PUBLIC METHODS                                      */
/* ======================================================================================== */

/* initializes the string, same as initializing with = { 0 }, which is preferable. 
 * the string is empty and NULL */
void vstr_new(Vstr *s) 
{
    s->cap = 0;
    s->len = 0;
}

/* new string with space for at most a string of length n. the string is empty but not NULL */
void vstr_init(Vstr *s, size_t n)
{
    vstr_new(s);
    vstr_reserve(s, n);
    vstr_cpy(s, "");
}

/* new string from c-style string */
void vstr_from(Vstr *s, const char *source)
{
    vstr_new(s);
    vstr_cpy(s, source);
}

/* clear variables, release memory */
void vstr_clear(Vstr *s) 
{
    if (s->cap)
        free(s->ptr);
    s->cap = 0;
    s->len = 0;
}

/* reserve memory for a string of at least <n> characters */
void vstr_reserve(Vstr *s, size_t n)
{
    if (n + 1 > s->cap)
        vs_resize(s, n + 1);
}

/* ensure the memory allocated is exactly as needed for the length */
void vstr_shrink_to_fit(Vstr *s)
{
    if (s->cap)
        vs_resize(s, s->len + 1);
}

/* strcpy of <source>
 * allocates memory as needed */
char *vstr_cpy(Vstr *dest, const char *source) 
{
    dest->len = strlen(source);
    vstr_reserve(dest, dest->len);
    return strcpy(dest->ptr, source);
}

/* strncpy of <source> of at most <n> characters
 * allocates memory as needed */
char *vstr_ncpy(Vstr *dest, const char *source, size_t n) 
{
    dest->len = strlen(source);
    if (dest->len > n)
        dest->len = n;
    vstr_reserve(dest, dest->len);
    dest->ptr[dest->len] = '\0';
    return strncpy(dest->ptr, source, dest->len);
}

/* strcat of <source>
 * allocates memory as needed */
char *vstr_cat(Vstr *dest, const char *source) 
{
    dest->len += strlen(source);
    vstr_reserve(dest, dest->len);
    return strcat(dest->ptr, source);
}

/* strncat of <source> of at most <n> characters
 * allocates memory as needed */
char *vstr_ncat(Vstr *dest, const char *source, size_t n) 
{
    size_t len_src;
    
    len_src = strlen(source);
    if (len_src < n)
        n = len_src;
    dest->len += n;
    vstr_reserve(dest, dest->len);
    dest->ptr[dest->len] = '\0';
    return strncat(dest->ptr, source, n);
}

/* merge <s2> into <s1> with c-string <sep> in between. releases the memory of <s2> */
char *vstr_merge(Vstr *s1, Vstr *s2, const char *sep) 
{
    if (s2->len) {
        s1->len += s2->len + strlen(sep);
        vstr_reserve(s1, s1->len);
        strcat(strcat(s1->ptr, sep), s2->ptr);
    }
    vstr_clear(s2);
    return s1->ptr;
}

/* return the underlying c-style string */
inline char *vstr_str(Vstr *s)
{
    if (s->cap)
        return s->ptr;
    return NULL;
}

/* strlen of the string */
inline size_t vstr_len(Vstr *s)
{
    return s->len;
}

/* amount of memory currently allocated for the string */
inline size_t vstr_capacity(Vstr *s)
{
    return s->cap;
}

/* iterate over characters of string.
 * call first vstr_iter_reset() to reset the interal counter
 * returns character at every loop, untill it returns '\0' */
/* TODO --- return char* so he caller can modify ? */
char vstr_iter(Vstr *s) 
{
    static size_t i = 0;

    if (s) {
        if (i < s->len)
            return s->ptr[i++];
    } else {
        i = 0;
    }

    return '\0';
}