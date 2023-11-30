#ifndef __VSTR_H__
#define __VSTR_H__

#include "gendef.h"

typedef struct vstr {
    char *ptr;
    size_t cap;
    size_t len;
} VStr;

void vstr_new(VStr *s);
void vstr_init(VStr *s, size_t n);
void vstr_from(VStr *s, const char *source);
void vstr_clear(VStr *s);
void vstr_reserve(VStr *s, size_t n);
void vstr_shrink_to_fit(VStr *s);
char *vstr_cpy(VStr *dest, const char *source);
char *vstr_ncpy(VStr *dest, const char *source, size_t n);
char *vstr_cat(VStr *dest, const char *source);
char *vstr_ncat(VStr *dest, const char *source, size_t n);
char *vstr_merge(VStr *s1, VStr *s2);
int vstr_cmp(VStr *s1, const char *s2);
int vstr_ncmp(VStr *s1, const char *s2, size_t n);
char *vstr_str(VStr *s);
size_t vstr_len(VStr *s);
size_t vstr_capacity(VStr *s);
bool vstr_empty(VStr *s);
char vstr_iter(VStr *s);

#define vstr_iter_reset()   vstr_iter(NULL, NULL)

#endif  /* __VSTR_H__ */