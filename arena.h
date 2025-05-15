#ifndef ARENA_H
#define ARENA_H

#include <stddef.h>
#include <stdint.h>

#define DEFAULT_REGION_SIZE 4096

struct region {
    struct region *next;
    size_t index;
    size_t count;
    uintptr_t data[];
};

struct arena {
    struct region *head;
};

void arena_free(struct arena *a);

void *arena_alloc(struct arena *a, size_t size_bytes);

char *arena_strdup(struct arena *a, const char *cstr);

char *arena_strndup(struct arena *a, const char *str, size_t len);

char *arena_format(struct arena *a, const char *fmt, ...);

#endif
