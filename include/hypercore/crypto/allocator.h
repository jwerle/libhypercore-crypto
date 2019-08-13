#ifndef HYPERCORE_CRYPTO_ALLOCATOR_H
#define HYPERCORE_CRYPTO_ALLOCATOR_H

#include "platform.h"

// Forward declarations
struct hypercore_crypto_allocator_stats_s;

/**
 * Allocator stats
 */
struct hypercore_crypto_allocator_stats_s {
  unsigned int alloc;
  unsigned int free;
};

/**
 * Returns allocator stats.
 */
HYPERCORE_CRYPTO_EXPORT const struct hypercore_crypto_allocator_stats_s
hypercore_crypto_allocator_stats();

/**
 * Returns number of allocations returned by `hypercore_crypto_alloc()`.
 */
HYPERCORE_CRYPTO_EXPORT int
hypercore_crypto_allocator_alloc_count();

/**
 * Returns number of deallocations by `hypercore_crypto_free()`.
 */
HYPERCORE_CRYPTO_EXPORT int
hypercore_crypto_allocator_free_count();

/**
 * Set the allocator function used in the library.
 * `malloc()=`
 */
HYPERCORE_CRYPTO_EXPORT void
hypercore_crypto_allocator_set(void *(*allocator)(unsigned long int));

/**
 * Set the deallocator function used in the library.
 * `free()=`
 */
HYPERCORE_CRYPTO_EXPORT void
hypercore_crypto_deallocator_set(void (*allocator)(void *));

/**
 * The allocator function used in the library.
 * Defaults to `malloc()`.
 */
HYPERCORE_CRYPTO_EXPORT void *
hypercore_crypto_alloc(unsigned long int);

/**
 * The deallocator function used in the library.
 * Defaults to `free()`.
 */
HYPERCORE_CRYPTO_EXPORT void
hypercore_crypto_free(void *);

#endif
