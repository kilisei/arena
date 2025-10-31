#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Arena {
  struct Arena *next;
  size_t capacity;
  size_t offset;
  uint8_t *bytes;
} Arena;

void arena_init(Arena *arena, size_t capacity) {
  arena->next = NULL;
  arena->offset = 0;
  arena->capacity = capacity;
  arena->bytes = malloc(capacity);
  if (arena->bytes == NULL) {
    fprintf(stderr, "malloc failed\n");
    arena->capacity = 0;
  }
}

void *arena_alloc(Arena *arena, size_t size) {
  if (arena == NULL || arena->bytes == NULL)
    return NULL;
  if (size == 0)
    return NULL;

  // find last arena
  Arena *last = arena;
  while (last->next != NULL) {
    last = last->next;
  }

  // fit in the last arena?
  if (last->offset + size <= last->capacity) {
    void *ptr = last->bytes + last->offset;
    last->offset += size;
    return ptr;
  }

  // need a new arena
  Arena *new_arena = malloc(sizeof(Arena));
  if (new_arena == NULL)
    return NULL;

  size_t new_capacity = arena->capacity;
  if (size > new_capacity) {
    new_capacity = size;
  }

  arena_init(new_arena, new_capacity);
  if (new_arena->bytes == NULL) {
    free(new_arena);
    return NULL;
  }

  last->next = new_arena;
  void *ptr = new_arena->bytes + new_arena->offset;
  new_arena->offset += size;
  return ptr;
}

void arena_free(Arena *arena) {
  if (arena == NULL)
    return;

  // free all linked arenas I think we don't want the first arena
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

void print_indent(size_t level) {
  for (size_t i = 0; i < level; ++i)
    fputs("  ", stdout);
}

void arena_print(Arena *arena) {
  if (arena == NULL) {
    printf("arena: (null)\n");
    return;
  }

  size_t level = 0;
  Arena *current = arena;
  while (current != NULL) {
    if (level > 0) {
      printf("\n");
    }
    print_indent(level);
    printf("Arena: %p\n", (void *)current);
    print_indent(level);
    printf("Capacity: %zu\n", current->capacity);
    print_indent(level);
    printf("Offset: %zu\n", current->offset);
    print_indent(level);
    printf("Next: %p\n", (void *)current->next);
    print_indent(level);
    printf("Bytes: %p\n", (void *)current->bytes);

    level++;
    current = current->next;
  }
}

int main(void) {
  Arena arena;
  arena_init(&arena, 512);
  arena_print(&arena);

  while (1) {
    printf("=================================================================\n");

    uint32_t size = 0;
    printf("bytes to alloc: ");
    fflush(stdout);

    if (scanf("%u", &size) != 1 || size <= 0) {
      fprintf(stderr, "Invalid input\n");
      arena_free(&arena);
      return EXIT_FAILURE;
    }

    void *ptr = arena_alloc(&arena, size * sizeof(uint8_t));
    if (ptr == NULL) {
      fprintf(stderr, "Allocation failed\n");
      arena_free(&arena);
      return EXIT_FAILURE;
    }
    arena_print(&arena);
  }

  arena_free(&arena);

  return EXIT_SUCCESS;
}
