#include "arena.h"

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
