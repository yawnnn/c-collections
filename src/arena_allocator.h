#ifndef __ARENA_ALLOCATOR_H__
#define __ARENA_ALLOCATOR_H__

typedef struct ArenaNode {
    struct ArenaNode *next;
} ArenaNode;

typedef struct Arena {
    ArenaNode *head;
} Arena;

#endif /* __ARENA_ALLOCATOR_H__ */