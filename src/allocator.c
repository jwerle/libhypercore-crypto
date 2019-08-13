#include "hypercore/crypto/allocator.h"
#include <stdlib.h>

#ifndef HYPERCORE_CRYPTO_ALLOCATOR_ALLOC
#define HYPERCORE_CRYPTO_ALLOCATOR_ALLOC 0
#endif

#ifndef HYPERCORE_CRYPTO_ALLOCATOR_FREE
#define HYPERCORE_CRYPTO_ALLOCATOR_FREE 0
#endif

static void *(*alloc)(unsigned long int) = HYPERCORE_CRYPTO_ALLOCATOR_ALLOC;
static void (*dealloc)(void *) = HYPERCORE_CRYPTO_ALLOCATOR_FREE;

static struct hypercore_crypto_allocator_stats_s stats = { 0 };

const struct hypercore_crypto_allocator_stats_s
hypercore_crypto_allocator_stats() {
  return (struct hypercore_crypto_allocator_stats_s) {
    .alloc = stats.alloc,
    .free = stats.free
  };
}

int
hypercore_crypto_allocator_alloc_count() {
  return stats.alloc;
}

int
hypercore_crypto_allocator_free_count() {
  return stats.free;
}

void
hypercore_crypto_allocator_set(void *(*allocator)(unsigned long int)) {
  alloc = allocator;
}

void
hypercore_crypto_deallocator_set(void (*deallocator)(void *)) {
  dealloc = deallocator;
}

void *
hypercore_crypto_alloc(unsigned long int size) {
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

void
hypercore_crypto_free(void *ptr) {
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
