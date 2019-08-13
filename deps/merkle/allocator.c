#include "merkle/allocator.h"
#include <stdlib.h>

#ifndef MERKLE_ALLOCATOR_ALLOC
#define MERKLE_ALLOCATOR_ALLOC 0
#endif

#ifndef MERKLE_ALLOCATOR_REALLOC
#define MERKLE_ALLOCATOR_REALLOC 0
#endif

#ifndef MERKLE_ALLOCATOR_FREE
#define MERKLE_ALLOCATOR_FREE 0
#endif

static void *(*alloc)(unsigned long int) = MERKLE_ALLOCATOR_ALLOC;
static void *(*re_alloc)(void *, unsigned long int) = MERKLE_ALLOCATOR_REALLOC;
static void (*dealloc)(void *) = MERKLE_ALLOCATOR_FREE;

static struct merkle_allocator_stats_s stats = { 0 };

const struct merkle_allocator_stats_s
merkle_allocator_stats() {
  return (struct merkle_allocator_stats_s) {
    .realloc = stats.realloc,
    .alloc = stats.alloc,
    .free = stats.free
  };
}

int
merkle_allocator_alloc_count() {
  return stats.alloc;
}

int
merkle_allocator_realloc_count() {
  return stats.realloc;
}

int
merkle_allocator_free_count() {
  return stats.free;
}

void
merkle_allocator_set(void *(*allocator)(unsigned long int)) {
  alloc = allocator;
}

void
merkle_reallocator_set(void *(*reallocator)(void*, unsigned long int)) {
  re_alloc = reallocator;
}

void
merkle_deallocator_set(void (*deallocator)(void *)) {
  dealloc = deallocator;
}

void *
merkle_alloc(unsigned long int size) {
  if (0 == size) {
    return 0;
  } else if (0 != alloc) {
    (void) stats.alloc++;
    return alloc(size);
  } else {
    (void) stats.alloc++;
    return malloc(size);
  }
}

void *
merkle_realloc(void *ptr, unsigned long int size) {
  if (0 == ptr || 0 == size) {
    return 0;
  } else if (0 != re_alloc) {
    (void) stats.realloc++;
    return re_alloc(ptr, size);
  } else {
    (void) stats.realloc++;
    return realloc(ptr, size);
  }
}

void
merkle_free(void *ptr) {
  if (0 == ptr) {
    return;
  } else if (0 != dealloc) {
    (void) stats.free++;
    dealloc(ptr);
  } else {
    (void) stats.free++;
    free(ptr);
  }
}
