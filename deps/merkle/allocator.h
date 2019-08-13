#ifndef MERKLE_ALLOCATOR_H
#define MERKLE_ALLOCATOR_H

#include "platform.h"

// Forward declarations
struct merkle_allocator_stats_s;

/**
 * Allocator stats
 */
struct merkle_allocator_stats_s {
  unsigned int realloc;
  unsigned int alloc;
  unsigned int free;
};

/**
 * Returns allocator stats.
 */
MERKLE_EXPORT const struct merkle_allocator_stats_s
merkle_allocator_stats();

/**
 * Returns number of allocations returned by `merkle_alloc()`.
 */
MERKLE_EXPORT int
merkle_allocator_alloc_count();

/**
 * Returns number of deallocations by `merkle_free()`.
 */
MERKLE_EXPORT int
merkle_allocator_free_count();

/**
 * Set the allocator function used in the library.
 * `malloc()=`
 */
MERKLE_EXPORT void
merkle_allocator_set(void *(*allocator)(unsigned long int));

/**
 * Set the reallocator function used in the library.
 * `realloc()=`
 */
MERKLE_EXPORT void
merkle_reallocator_set(void *(*reallocator)(void *, unsigned long int));

/**
 * Set the deallocator function used in the library.
 * `free()=`
 */
MERKLE_EXPORT void
merkle_deallocator_set(void (*allocator)(void *));

/**
 * The allocator function used in the library.
 * Defaults to `malloc()`.
 */
MERKLE_EXPORT void *
merkle_alloc(unsigned long int);

/**
 * The reallocator function used in the library.
 * Defaults to `realloc()`.
 */
MERKLE_EXPORT void *
merkle_realloc(void *ptr, unsigned long int size);

/**
 * The deallocator function used in the library.
 * Defaults to `free()`.
 */
MERKLE_EXPORT void
merkle_free(void *);

#endif
