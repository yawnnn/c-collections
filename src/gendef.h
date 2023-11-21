#ifndef __GENDEF_H__
#define __GENDEF_H__

/* General include. To simplify development and testing */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    NO = 0,
    YES,
} bool;

#ifndef NULL
#define NULL ((void *)0)
#endif

#define CRED     "\x1b[31m"
#define CGREEN   "\x1b[32m"
#define CYELLOW  "\x1b[33m"
#define CBLUE    "\x1b[34m"
#define CMAGENTA "\x1b[35m"
#define CCYAN    "\x1b[36m"
#define CRESET   "\x1b[0m"

#define __DBG__     1

#endif  /* __GENDEF_H__ */