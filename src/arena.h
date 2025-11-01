#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct Arena {
  struct Arena *next;
  size_t capacity;
  size_t offset;
  uint8_t *bytes;
} Arena;

bool arena_init(Arena *arena, size_t capacity);

void *arena_alloc(Arena *arena, size_t size);

void arena_reset(Arena *arena);

void arena_free(Arena *arena);

void arena_print(const Arena *arena);
