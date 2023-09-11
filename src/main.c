#include "vector.h"
#include "vstr.h"

char *print_itos(void *pval, char *buf)
{
    if (buf == NULL)
        buf = (char *)malloc(12 + 1);
    sprintf(buf, "%d", *((int*)pval));

    return buf;
}

int main() 
{
    Vec v;

    int arr[] = {9,8,7,6};
    int elem;
    printf("TESTING VECTOR OF INTEGERS...\n");

    vec_new(&v, sizeof(elem));

    elem = 2;
    vec_push(&v, &elem);
    elem = 4;
    vec_push(&v, &elem);
    elem = -1;
    vec_push(&v, &elem);

    vec_dbg(&v);
    vec_print(&v, print_itos);

    vec_drop(&v);

    vec_from(&v, arr, sizeof(arr)/sizeof(int), sizeof(int));
    vec_print(&v, print_itos);

    vec_drop(&v);

    // VStr s;
    // char s1[] = "First string";
    // char s2[] = "Second string";
    // char s3[] = "Thrist and final string";

    // printf("TESTING VSTR...\n");

    // vstr_new(&s);
    // vstr_cpy(&s, s1);
    // printf("%s", s);

    // vec_new(&v, sizeof(Vec));

    // vstr_from(&s, s1);
    // vec_push_n(&s, s1, sizeof(s1));
    // vec_push(&v, &s);

    // vec_new_with(&s, sizeof(s2), sizeof(char));
    // vec_push_n(&s, s2, sizeof(s2));
    // vec_push(&v, &s);

    // vec_new_with(&s, sizeof(s3), sizeof(char));
    // vec_push_n(&s, s3, sizeof(s3));
    // vec_push(&v, &s);

    // vec_print(&v, NULL);

    // vec_iter_reset();
    // while (vec_iter(&v, &s))
    //     vec_drop(&s);
    // vec_drop(&v);
}