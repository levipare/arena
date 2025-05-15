#include "arena.h"

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct region *region_new(size_t size_bytes) {
    // how many words are required to fit size_bytes
    size_t count = (size_bytes + sizeof(uintptr_t) - 1) / sizeof(uintptr_t);

    // byte size of region
    size_t size = sizeof(struct region) + count * sizeof(uintptr_t);

    struct region *r = malloc(size); // TODO: USE MMAP
    r->next = NULL;
    r->index = 0;
    r->count = count;

    return r;
}

void arena_free(struct arena *a) {
    struct region *r = a->head;
    while (r) {
        struct region *tmp = r;
        r = r->next;
        free(tmp);
    }
}

void *arena_alloc(struct arena *a, size_t size_bytes) {
    // equivalent to ceil(size_bytes / sizeof(uintptr_t))
    size_t count = (size_bytes + sizeof(uintptr_t) - 1) / sizeof(uintptr_t);

    size_t region_size = DEFAULT_REGION_SIZE;
    if (count * sizeof(uintptr_t) > DEFAULT_REGION_SIZE) {
        region_size = count * sizeof(uintptr_t);
    }

    if (!a->head) {
        a->head = region_new(region_size);
    }

    struct region *r = a->head;
    while (r->index + count > r->count && r->next) {
        r = r->next;
    }

    if (r->index + count > r->count) {
        r = r->next = region_new(region_size);
    }

    void *p = &r->data[r->index];
    r->index += count;

    return p;
}

char *arena_strdup(struct arena *a, const char *cstr) {
    size_t len = strlen(cstr);
    char *s = arena_alloc(a, len + 1);
    memcpy(s, cstr, len);
    s[len] = '\0';

    return s;
}

char *arena_strndup(struct arena *a, const char *str, size_t len) {
    char *s = arena_alloc(a, len + 1);
    memcpy(s, str, len);
    s[len] = '\0';

    return s;
}

char *arena_format(struct arena *a, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int size = vsnprintf(NULL, 0, fmt, args);
    va_end(args);

    char *str = arena_alloc(a, size + 1);

    va_start(args, fmt);
    vsnprintf(str, size + 1, fmt, args);
    va_end(args);

    return str;
}
