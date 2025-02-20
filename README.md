# Arena Allocator

A simple arena allocator implementation in C that includes a few common utility
functions to use in tandem with arena allocators.

## Example Usage
```c
#include "arena.h"

int main() {
    Arena a = {0};

    int *x_p = arena_alloc(&a, sizeof(int));
    *x_p = 10;

    char *s = arena_strndup(&a, "Hello World", 11);

    arena_free(&a);
    return 0;
}
```
