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

static void vstr_dbg(VStr *s)
{
    printf("VStr => s: %s, cap: %u, len: %u, strlen: %d\n", s->ptr, s->cap, s->len, s->ptr && s->cap ? (int)strlen(s->ptr) : (int)-1);
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

inline static void vs_alloc(VStr *s, size_t n)
{
    s->ptr = malloc(n);
    s->cap = n;
}

inline static void vs_realloc(VStr *s, size_t n)
{
    s->ptr = realloc(s->ptr, n);
    s->cap = n;
}

/* Increase capacity by GROWTH_FACTOR */
static void vs_grow(VStr *s) 
{
    __DBG_PRINT_BEFORE(s);
    if (s->cap == 0)
        vs_alloc(s, GROWTH_FACTOR);
    else
        vs_realloc(s, s->cap * GROWTH_FACTOR);
    __DBG_PRINT_AFTER(s);
}

/* Grow where possible, otherwise realloc */
static void vs_resize(VStr *s, size_t n) 
{
    __DBG_PRINT_BEFORE(s);
    if (s->cap) {
        if (n < s->cap || n > s->cap * GROWTH_FACTOR)
            vs_realloc(s, n);
        else if (n > s->cap)
            vs_grow(s);
    } else {
        vs_alloc(s, n);
    }
    __DBG_PRINT_AFTER(s);
}

/* ======================================================================================== */
/*                                      PUBLIC METHODS                                      */
/* ======================================================================================== */

/* initializes the string, same as initializing with = { 0 }, which is preferable. 
 * the string is empty and NULL */
void vstr_new(VStr *s) 
{
    s->cap = 0;
    s->len = 0;
}

/* new string with space for at most a string of length n. the string is empty but not NULL */
void vstr_init(VStr *s, size_t n)
{
    vstr_new(s);
    vstr_reserve(s, n);
    vstr_cpy(s, "");
}

/* new string from c-style string */
void vstr_from(VStr *s, const char *source)
{
    vstr_new(s);
    vstr_cpy(s, source);
}

/* clear variables, release memory */
void vstr_clear(VStr *s) 
{
    if (s->cap)
        free(s->ptr);
    s->cap = 0;
    s->len = 0;
}

/* reserve memory for a string of at least <n> characters */
void vstr_reserve(VStr *s, size_t n)
{
    if (n + 1 > s->cap)
        vs_resize(s, n + 1);
}

/* ensure the memory allocated is exactly as needed for the length */
void vstr_shrink_to_fit(VStr *s)
{
    if (s->cap)
        vs_resize(s, s->len + 1);
}

/* strcpy of <source>
 * allocates memory as needed */
/* TODO --- Without debug mode, i don't need char *ret; */
char *vstr_cpy(VStr *dest, const char *source) 
{
    __DBG_PRINT_BEFORE(dest);
    char *ret;

    dest->len = strlen(source);
    vstr_reserve(dest, dest->len);
    ret = strcpy(dest->ptr, source);

    __DBG_PRINT_AFTER(dest);
    return ret;
}

/* strncpy of <source> of at most <n> characters
 * allocates memory as needed */
char *vstr_ncpy(VStr *dest, const char *source, size_t n) 
{
    __DBG_PRINT_BEFORE(dest);
    char *ret;

    dest->len = strlen(source);
    if (dest->len > n)
        dest->len = n;
    vstr_reserve(dest, dest->len);
    ret = strncpy(dest->ptr, source, dest->len);
    dest->ptr[dest->len] = '\0';

    __DBG_PRINT_AFTER(dest);
    return ret;
}

/* strcat of <source>
 * allocates memory as needed */
char *vstr_cat(VStr *dest, const char *source) 
{
    __DBG_PRINT_BEFORE(dest);
    char *ret;

    dest->len += strlen(source);
    vstr_reserve(dest, dest->len);
    ret =  strcat(dest->ptr, source);

    __DBG_PRINT_AFTER(dest);
    return ret;
}

/* strncat of <source> of at most <n> characters
 * allocates memory as needed */
char *vstr_ncat(VStr *dest, const char *source, size_t n) 
{
    __DBG_PRINT_BEFORE(dest);
    char *ret;
    size_t len_src;
    
    len_src = strlen(source);
    dest->len += len_src > n ? n : len_src;
    vstr_reserve(dest, dest->len);
    ret = strncat(dest->ptr, source, dest->len);
    dest->ptr[dest->len] = '\0';

    __DBG_PRINT_AFTER(dest);
    return ret;
}

/* merge <s2> into <s1>. releases the memory of <s2> */
/* TODO --- merge -> join (with <sSeparator>) */
char *vstr_merge(VStr *s1, VStr *s2) 
{
    __DBG_PRINT_BEFORE(s1);
    if (s2->cap)
        vstr_cpy(s1, s2->ptr);
    vstr_clear(s2);
    __DBG_PRINT_AFTER(s1);
    return s1->ptr;
}

/* strcmp between <s1> and <s2> */
/* TODO --- check s->cap ? */
inline int vstr_cmp(VStr *s1, const char *s2)
{
    return strcmp(s1->ptr, s2);
}

/* strncmp between <s1> and <s2> of at most <n> characters */
inline int vstr_ncmp(VStr *s1, const char *s2, size_t n)
{
    return strncmp(s1->ptr, s2, n);
}

/* return the c-style string */
inline char *vstr_str(VStr *s)
{
    if (s->cap)
        return s->ptr;
    return NULL;
}

/* strlen of the string */
inline size_t vstr_len(VStr *s)
{
    return s->len;
}

/* amount of memory currently allocated for the string */
inline size_t vstr_capacity(VStr *s)
{
    return s->cap;
}

/* if the string is empty */
inline bool vstr_empty(VStr *s)
{
    return s->len == 0;
}

/* iterate over characters of string.
 * call first vstr_iter_reset() to reset the interal counter
 * returns character at every loop, untill it returns '\0' */
/* TODO --- return char* so he caller can modify ? */
char vstr_iter(VStr *s) 
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