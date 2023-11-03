#include "vector.h"
#include "vstr.h"

int main() 
{
    // Vec v;

    // int arr[] = {9,8,7,6};
    // int elem;
    // printf("\nTESTING VECTOR OF INTEGERS...\n");

    // vec_new(&v, sizeof(elem));

    // elem = 2;
    // vec_push(&v, &elem);
    // elem = 4;
    // vec_push(&v, &elem);
    // elem = -1;
    // vec_push(&v, &elem);

    // vec_dbg(&v);
    // vec_sort(&v, VEC_SORT_ASC);
    // vec_sort(&v, VEC_SORT_DESC);

    // vec_drop(&v);

    // vec_from(&v, arr, sizeof(arr)/sizeof(int), sizeof(int));

    // vec_drop(&v);

    // VStr s;
    // char s1[] = "First string";
    // char s2[] = "Second string";
    // char s3[] = "Third and final string";

    // printf("\nTESTING VSTR...\n");

    // vstr_new(&s);
    // vstr_cpy(&s, s1);
    // vstr_ncpy(&s, s2, 2);
    // vstr_cat(&s, "__AND__");
    // vstr_ncat(&s, s3, 2);

    // vstr_drop(&s);

    // printf("\nTESTING TYPE COERCION (SORT OF)...\n");

    // vec_new(&v, sizeof(char *));

    // vstr_from(&s, s1);
    // vstr_cpy(&s, s2);
    // vec_push(&v, &s);

    // vstr_init(&s, sizeof(s2));
    // vstr_ncpy(&s, s2, sizeof(s2) -2);
    // vec_push(&v, &s);

    // vstr_from(&s, s2);
    // vstr_ncat(&s, s3, sizeof(s3) - 4);
    // vec_push(&v, &s);

    // vec_iter_reset();
    // while (vec_iter(&v, &s))
    //     free(*((char **)&s));
    // vec_drop(&v);

    /* TODO --- Write proper tests */

    return 0;
}