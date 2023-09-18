#ifndef __VSTR_H__
#define __VSTR_H__

#include "gendef.h"

typedef struct vstr {
    char *ptr;
    unsigned int cap;
    unsigned int len;    
} VStr;

/* TODO --- Implement strxxx() manually? */

void vstr_new(VStr *s);
void vstr_init(VStr *s, unsigned int cap);
void vstr_from(VStr *s, const char *source);
void vstr_drop(VStr *s);
void vstr_space_for(VStr *s, unsigned int chars);
char *vstr_get(VStr *s);
char *vstr_cpy(VStr *dest, const char *source);
char *vstr_ncpy(VStr *dest, const char *source, unsigned int n);
char *vstr_cat(VStr *dest, const char *source);
char *vstr_ncat(VStr *dest, const char *source, unsigned int n);
int  vstr_cmp(VStr *s1, const char *s2);
int  vstr_ncmp(VStr *s1, const char *s2, unsigned int n);
void vstr_merge(VStr *s1, VStr *s2);
char *vstr_as_str(VStr *s);
char vstr_iter(VStr *s);
void vstr_dbg(VStr *s);

#endif  /* __VSTR_H__ */