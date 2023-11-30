#ifndef __VSTR_H__
#define __VSTR_H__

#include "gendef.h"

typedef struct {
    char *ptr;
    size_t cap;
    size_t len;
} Vstr;

void vstr_new(Vstr *s);
void vstr_init(Vstr *s, size_t n);
void vstr_from(Vstr *s, const char *source);
void vstr_clear(Vstr *s);
void vstr_reserve(Vstr *s, size_t n);
void vstr_shrink_to_fit(Vstr *s);
char *vstr_cpy(Vstr *dest, const char *source);
char *vstr_ncpy(Vstr *dest, const char *source, size_t n);
char *vstr_cat(Vstr *dest, const char *source);
char *vstr_ncat(Vstr *dest, const char *source, size_t n);
char *vstr_merge(Vstr *s1, Vstr *s2, const char *sep);
char *vstr_str(Vstr *s);
size_t vstr_len(Vstr *s);
size_t vstr_capacity(Vstr *s);
char vstr_iter(Vstr *s);

#define vstr_iter_reset()   vstr_iter(NULL, NULL)

#endif  /* __VSTR_H__ */