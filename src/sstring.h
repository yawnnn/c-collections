/**
 * @file sstring.h
 */

#ifndef __VSTR_H__
#define __VSTR_H__

#include <stdbool.h>
#include <stdlib.h>

/**
 * @brief Dynamic string
 */
typedef struct SStr {
    char *ptr;  /**< underlying c-style string (access through sstr_data()) */
    size_t cap; /**< capacity allocated */
    size_t len; /**< length of the SStr */
} SStr;

/**
 * @brief new SStr
 *
 * the SStr is not allocated, therefore sstr_data() returns NULL
 *
 * @param s SStr
 */
void sstr_new(SStr *s);

/**
 * @brief new SStr with reserved space
 *
 * the SStr has 0 length but is allocated, therefore sstr_data() is valid
 *
 * @param s SStr
 * @param len minimum number of characters to reserve memory for
 */
void sstr_new_with(SStr *s, size_t len);

/**
 * @brief new SStr from c-style string
 *
 * @param s SStr
 * @param source source c-style string
 */
void sstr_from(SStr *s, const char *source);

/**
 * @brief release memory
 *
 * @param s SStr
 */
void sstr_free(SStr *s);

/**
 * @brief empty the string but don't free the memory, so it can be reused
 *
 * @param s SStr
 */
inline void sstr_truncate(SStr *s) {
    if (s->len) {
        *s->ptr = '\0';
        s->len = 0;
    }
}

/**
 * @brief reserve memory ahead of time
 *
 * @param s SStr
 * @param len minimum number of characters to reserve memory for
 */
void sstr_reserve(SStr *s, size_t len);

/**
 * @brief shrink allocated memory to what is exactly needed for length
 *
 * @param s SStr
 */
void sstr_shrink_to_fit(SStr *s);

/**
 * @brief return the underlying c-style string, or NULL
 *
 * @param s SStr
 * @return c-style string or NULL
 */
inline char *sstr_data(SStr *s) {
    if (s->cap)  // len could be 0, but still allocated because of the null-terminating character
        return s->ptr;
    return NULL;
}

/**
 * @brief return the underlying c-style string starting at @p pos, or NULL
 *
 * @param s SStr
 * @param pos start position
 * @return c-style string or NULL
 */
inline char *sstr_data_from(SStr *s, size_t pos) {
    // asking for the position from the null-terminating char is valid, so i hate to check s->cap too
    if (pos <= s->len && s->cap)
        return s->ptr + pos;
    return NULL;
}

/**
 * @brief strcpy for SStr
 *
 * @param dest SStr
 * @param source source c-style string
 * @return the underlying c-style string of @p dest
 */
char *sstr_cpy(SStr *dest, const char *source);

/**
 * @brief strncpy for SStr
 *
 * @param dest SStr
 * @param source source c-style string
 * @param num max number of characters to copy
 * @return the underlying c-style string of @p dest
 */
char *sstr_ncpy(SStr *dest, const char *source, size_t num);

/**
 * @brief strcat for SStr
 *
 * @param dest SStr
 * @param source source c-style string
 * @return the underlying c-style string of @p dest
 */
char *sstr_cat(SStr *dest, const char *source);

/**
 * @brief strncat for SStr
 *
 * @param dest SStr
 * @param source source c-style string
 * @param num max number of characters to concat
 * @return the underlying c-style string of @p dest
 */
char *sstr_ncat(SStr *dest, const char *source, size_t num);

/**
 * @brief merge two SStr
 *
 * merge @p source into @p dest with c-style string @p sep in between. consumes @p source
 *
 * @param dest SStr dest
 * @param source SStr source
 * @return the underlying c-style string of @p dest
 */
char *sstr_merge(SStr *dest, SStr *source, const char *sep);

/**
 * @brief if SStr is empty
 *
 * @param s SStr
 * @return boolean
 */
inline bool sstr_is_empty(SStr *s) {
    return s->len == 0;
}

#endif /* __VSTR_H__ */