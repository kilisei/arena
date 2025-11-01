#include "arena.c"
#include "arena.h"

int main(void) {
  Arena arena;
  arena_init(&arena, 1024);

  arena_alloc(&arena, 1024 * 1024);
  arena_print(&arena);

  arena_free(&arena);
  return 0;
}
