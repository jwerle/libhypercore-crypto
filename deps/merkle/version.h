#ifndef MERKLE_VERSION_H
#define MERKLE_VERSION_H

#include "platform.h"

/**
 * Returns the version string for the library.
 */
MERKLE_EXPORT const char *
merkle_version_string();

/**
 * Returns the 32 bit version number that encodes the
 * major, minor, patch, and revision version parts.
 */
MERKLE_EXPORT const unsigned long int
merkle_version();

/**
 * Returns the major version part.
 */
MERKLE_EXPORT const unsigned long int
merkle_version_major();

/**
 * Returns the minor version part.
 */
MERKLE_EXPORT const unsigned long int
merkle_version_minor();

/**
 * Returns the minor patch part.
 */
MERKLE_EXPORT const unsigned long int
merkle_version_patch();

/**
 * Returns the minor revision part.
 */
MERKLE_EXPORT const unsigned long int
merkle_version_revision();

#endif
