#ifndef ARENA_H
#define ARENA_H

#include <stddef.h>
#include <stdint.h>

#define DEFAULT_REGION_SIZE 4096

typedef struct Region {
    struct Region *next;
    size_t index;
    size_t count;
    uintptr_t data[];
} Region;

typedef struct Arena {
    Region *head;
} Arena;

void arena_free(Arena *a);

void *arena_alloc(Arena *a, size_t size_bytes);

char *arena_strdup(Arena *a, const char *cstr);

char *arena_strndup(Arena *a, const char *str, size_t len);

char *arena_format(Arena *a, const char *fmt, ...);

#endif
