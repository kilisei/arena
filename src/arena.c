#include "arena.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

bool arena_init(Arena *arena, size_t capacity) {
  if (arena == NULL) {

    return false;
  }

  arena->next = NULL;
  arena->offset = 0;
  arena->capacity = capacity;

  if (capacity == 0) {
    arena->bytes = NULL;
    return true;
  }

  arena->bytes = malloc(capacity);
  if (arena->bytes == NULL) {
    arena->capacity = 0;
    return false;
  }

  return true;
}

void *arena_alloc(Arena *arena, size_t size) {
  if (arena == NULL || arena->bytes == NULL) {
    return NULL;
  }
  if (size == 0) {
    return NULL;
  }

  Arena *last = arena;
  while (last->next != NULL) {
    last = last->next;
  }

  if (last->offset + size <= last->capacity) {
    void *ptr = last->bytes + last->offset;
    last->offset += size;
    return ptr;
  }

  Arena *new_arena = malloc(sizeof(Arena));
  if (new_arena == NULL) {
    return NULL;
  }

  size_t new_capacity = arena->capacity;
  if (size > new_capacity) {
    new_capacity = size;
  }

  if (arena_init(new_arena, new_capacity)) {
    free(new_arena);
    return NULL;
  }

  last->next = new_arena;
  void *ptr = new_arena->bytes + new_arena->offset;
  new_arena->offset += size;
  return ptr;
}

void arena_free(Arena *arena) {
  if (arena == NULL) {
    return;
  }

  Arena *current = arena->next;
  while (current != NULL) {
    printf("free: %p\n", (void *)current);
    Arena *next = current->next;
    if (current->bytes) {
      free(current->bytes);
    }
    free(current);
    current = next;
  }

  // free only the bytes off the first one
  if (arena->bytes) {
    free(arena->bytes);
    arena->bytes = NULL;
  }
  arena->next = NULL;
  arena->capacity = 0;
  arena->offset = 0;
}

void arena_reset(Arena *arena) {
  if (arena == NULL) {
    return;
  }

  Arena *cur = arena;
  while (cur != NULL) {
    cur->offset = 0;
    cur = cur->next;
  }
}

void print_indent(const char *format, size_t level, ...) {
  va_list args;
  va_start(args, level);

  for (size_t i = 0; i < level; ++i) {
    fputs(" ", stdout);
  }

  vprintf(format, args);
  va_end(args);
}

void arena_print(const Arena *arena) {
  if (arena == NULL) {
    printf("arena: (nil)\n");
    return;
  }

  size_t level = 0;
  const Arena *current = arena;
  while (current != NULL) {
    if (level > 0) {
      printf("\n");
    }

    print_indent("Arena: %p\n", level, (const void *)current);
    print_indent("Capacity: %zu\n", level, current->capacity);
    print_indent("Offset: %zu\n", level, current->offset);
    print_indent("Next: %p\n", level, (const void *)current->next);
    print_indent("Bytes: %p\n", level, current->bytes);

    level++;
    current = current->next;
  }
}
