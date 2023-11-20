#include "vector.h"
#include "vstr.h"

// void vec_dbg(Vec *v)
// {
//     printf("Vec => ptr: " CCYAN "%p" CRESET ", cap: %zu, len: %zu, size: %zu\n", v->ptr, v->cap, v->len, v->size);
// }

// void dbg(Vec *v)
// {
//     int d;

//     vec_dbg(v);

//     for (vec_iter(NULL, NULL); vec_iter(v, &d); )
//         printf("%i, ", d);
//     printf("\n");
// }

int main() 
{
    // Vec v;
    // int t[] = { 1, INT_MAX - 1, 34, 45 };
    // int t2[] = { 55, 66 };
    // int d = 2;

    // vec_new_from(&v, sizeof(int), t, sizeof(t)/sizeof(int));
    // vec_pop(&v, NULL);
    // vec_insert(&v, &d, 0);
    // vec_insert_n(&v, t2, 3, 2);
    // dbg(&v);
    // vec_remove(&v, 2, &d);
    // dbg(&v);
    // printf("remove - %i\n", d);
    // d = 98;
    // vec_push(&v, &d);
    // d = 99;
    // vec_push(&v, &d);
    // dbg(&v);

    // int t3[10];
    // vec_remove_n(&v, v.len - 4, 2, t3);
    // dbg(&v);
    // printf("remove - %i %i\n", t3[0], t3[1]);

    return 0;
}