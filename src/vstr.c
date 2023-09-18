#include "gendef.h"
#include "vstr.h"

/* ================== BEHAVIOR ================== */

#define GROWTH_FACTOR   (2UL)
#define RESIZE_EXACT_GROWTH_FACTOR  (1)

/* ================== DEBUG ================== */

#if __DBG__
#define __STR_DEBUG_MODE 1
#else
#define __STR_DEBUG_MODE 0
#endif

static const char max_depth_tabs[20] = "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t";
static short int __dbg_depth = 0;

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

/* ================== METHODS ================== */

static void vstr_set_capacity(VStr *s, unsigned int cap) 
{
    __DBG_PRINT_BEFORE(s);
    if (s->cap == 0)
        s->ptr = malloc(cap);
    else
        s->ptr = realloc(s->ptr, cap);
    s->cap = cap;
    __DBG_PRINT_AFTER(s);
}

static void vstr_grow(VStr *s) 
{
    __DBG_PRINT_BEFORE(s);
    if (s->cap == 0) {
        s->ptr = malloc(1);
        s->cap = 1;
    } else {
        s->ptr = realloc(s->ptr, s->cap * GROWTH_FACTOR);
        s->cap *= GROWTH_FACTOR;
    }
    __DBG_PRINT_AFTER(s);
}

static void vstr_resize(VStr *s, unsigned int min) 
{
    __DBG_PRINT_BEFORE(s);

#if RESIZE_EXACT_GROWTH_FACTOR
    if (min < s->cap)
        vstr_set_capacity(s, min);
    else if (min > s->cap)
        vstr_set_capacity(s, min + min - (s->cap * GROWTH_FACTOR));
#else
    if (min < s->cap || min > s->cap * GROWTH_FACTOR)
        vstr_set_capacity(s, min);
    else if (s->cap < min)
        vstr_grow(s);
#endif

    __DBG_PRINT_AFTER(s);
}

void vstr_space_for(VStr *s, unsigned int chars)
{
    if (s->cap < chars + 1)
        vstr_resize(s, chars + 1);
}

void vstr_new(VStr *s) 
{
    s->ptr = NULL;
    s->cap = 0;
    s->len = 0;
}

void vstr_init(VStr *s, unsigned int sz)
{
    s->cap = 0;
    s->len = 0;
    vstr_set_capacity(s, sz + 1);
    *s->ptr = '\0';
}

void vstr_from(VStr *s, const char *source)
{
    vstr_new(s);
    vstr_cpy(s, source);
}

void vstr_drop(VStr *s) 
{
    s->cap = 0;
    s->len = 0;
    if (s->cap)
        free(s->ptr);
}

char *vstr_get(VStr *s)
{
    if (s->cap)
        return s->ptr;
    return NULL;
}

/* TODO --- Without debug mode, i don't need char *ret; */
char *vstr_cpy(VStr *dest, const char *source) 
{
    __DBG_PRINT_BEFORE(dest);
    char *ret;

    dest->len = strlen(source);
    vstr_space_for(dest, dest->len);
    ret = strcpy(dest->ptr, source);

    __DBG_PRINT_AFTER(dest);
    return ret;
}

char *vstr_ncpy(VStr *dest, const char *source, unsigned int n) 
{
    __DBG_PRINT_BEFORE(dest);
    char *ret;

    dest->len = strlen(source);
    if (dest->len > n)
        dest->len = n;
    vstr_space_for(dest, dest->len);
    dest->ptr[dest->len] = '\0';
    ret = strncpy(dest->ptr, source, n);

    __DBG_PRINT_AFTER(dest);
    return ret;
}

char *vstr_cat(VStr *dest, const char *source) 
{
    __DBG_PRINT_BEFORE(dest);
    char *ret;

    dest->len += strlen(source);
    vstr_space_for(dest, dest->len);
    ret =  strcat(dest->ptr, source);

    __DBG_PRINT_AFTER(dest);
    return ret;
}

char *vstr_ncat(VStr *dest, const char *source, unsigned int n) 
{
    __DBG_PRINT_BEFORE(dest);
    char *ret;

    unsigned int start = dest->len;
    dest->len += strlen(source);
    if (dest->len > start + n)
        dest->len = start + n;
    vstr_space_for(dest, dest->len);
    dest->ptr[dest->len] = '\0';
    ret =  strncat(dest->ptr, source, n);

    __DBG_PRINT_AFTER(dest);
    return ret;
}

/* TODO --- check s->cap, and give error */
int vstr_cmp(VStr *s1, const char *s2)
{
    return strcmp(s1->ptr, s2);
}

int vstr_ncmp(VStr *s1, const char *s2, unsigned int n)
{
    return strncmp(s1->ptr, s2, n);
}

void vstr_merge(VStr *s1, VStr *s2) 
{
    __DBG_PRINT_BEFORE(s1);
    if (s1->cap && s2->cap)
        vstr_cpy(s1, s2->ptr);
    vstr_drop(s2);
    __DBG_PRINT_AFTER(s1);
}

char *vstr_as_str(VStr *s)
{
    return s->ptr;
}

char vstr_iter(VStr *s) 
{
    static unsigned int i = 0;

    if (s) {
        if (i < s->len)
            return s->ptr[i++];
    } else {
        i = 0;
    }

    return '\0';
}

void vstr_dbg(VStr *s)
{
    printf("VStr => s: %s, cap: %u, len: %u, strlen: %d\n", s->ptr, s->cap, s->len, s->ptr && s->cap ? (int)strlen(s->ptr) : (int)-1);
}