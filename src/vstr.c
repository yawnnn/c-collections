#include "gendef.h"
#include "vstr.h"

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

#define GROWTH_FACTOR   (2UL)

#define s_getlen(s)         ((s)->len - 1)
#define s_setlen(s, ln)     ((s)->len = ln + 1)
#define s_azzlen(s)         ((s)->len = 0)
#define s_is_init(s)        ((s)->len > 0)

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

static void vstr_resize_to_len(VStr *s)
{
    vstr_resize(s, s->len);
}

void vstr_new(VStr *s) 
{
    s->ptr = NULL;
    s->cap = 0;
    s_azzlen(s);
}

void vstr_init(VStr *s, unsigned int cap)
{
    s->cap = 0;
    vstr_set_capacity(s, cap);
    s_setlen(s, 0);
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
    s_azzlen(s);
    if (s->cap)
        free(s->ptr);
}

void vstr_resize(VStr *s, unsigned int min) 
{
    __DBG_PRINT_BEFORE(s);
    if (s->cap > min || s->cap * GROWTH_FACTOR < min)
        vstr_set_capacity(s, min);
    else if (s->cap < min)
        vstr_grow(s);
    __DBG_PRINT_AFTER(s);
}

char *vstr_get(VStr *s)
{
    if (s_is_init(s))
        return s->ptr;
    return NULL;
}

/* TODO --- Implement strxxx() manually? */

char *vstr_cpy(VStr *dest, const char *source) 
{
    s_setlen(dest, strlen(source));
    vstr_resize_to_len(dest);
    return strcpy(dest->ptr, source);
}

char *vstr_ncpy(VStr *dest, const char *source, unsigned int len) 
{
    s_setlen(dest, strlen(source));
    if (s_getlen(dest) > len)
        s_setlen(dest, len);
    vstr_resize_to_len(dest);
    dest->ptr[s_getlen(dest)] = '\0';
    return strncpy(dest->ptr, source, len);
}

char *vstr_cat(VStr *dest, const char *source) 
{
    s_setlen(dest, s_getlen(dest) + strlen(source));
    vstr_resize_to_len(dest);
    return strcat(dest->ptr, source);
}

char *vstr_ncat(VStr *dest, const char *source, unsigned int len) 
{
    unsigned int start = s_getlen(dest);
    s_setlen(dest, s_getlen(dest) + strlen(source));
    if (s_getlen(dest) > start + len)
        s_setlen(dest, start + len);
    vstr_resize_to_len(dest);
    dest->ptr[s_getlen(dest)] = '\0';
    return strncat(dest->ptr, source, len);
}

/* TODO --- check s_is_init(s1) */
int vstr_cmp(VStr *s1, const char *s2)
{
    return strcmp(s1->ptr, s2);
}

int vstr_ncmp(VStr *s1, const char *s2, unsigned int len)
{
    return strncmp(s1->ptr, s2, len);
}

void vstr_merge(VStr *s1, VStr *s2) 
{
    __DBG_PRINT_BEFORE(s1);
    if (s_is_init(s1) && s_is_init(s2))
        vstr_cpy(s1, s2->ptr);
    vstr_drop(s2);
    __DBG_PRINT_AFTER(s1);
}

char vstr_iter(VStr *s) 
{
    static unsigned int i = 0;

    if (s) {
        if (i < s->len - 1)
            return s->ptr[i++];
    } else {
        i = 0;
    }

    return '\0';
}

void vstr_dbg(VStr *s)
{
    printf("VStr => s: %s, cap: %u, len: %u, strlen: %d\n", s->ptr, s->cap, s->len, s->ptr ? (int)strlen(s->ptr) : (int)-1);
}