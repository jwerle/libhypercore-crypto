#ifndef HYPERCORE_CRYPTO_VERSION_H
#define HYPERCORE_CRYPTO_VERSION_H

#include "platform.h"

/**
 * Returns the version string for the library.
 */
HYPERCORE_CRYPTO_EXPORT const char *
hypercore_crypto_version_string();

/**
 * Returns the 32 bit version number that encodes the
 * major, minor, patch, and revision version parts.
 */
HYPERCORE_CRYPTO_EXPORT const unsigned long int
hypercore_crypto_version();

/**
 * Returns the major version part.
 */
HYPERCORE_CRYPTO_EXPORT const unsigned long int
hypercore_crypto_version_major();

/**
 * Returns the minor version part.
 */
HYPERCORE_CRYPTO_EXPORT const unsigned long int
hypercore_crypto_version_minor();

/**
 * Returns the minor patch part.
 */
HYPERCORE_CRYPTO_EXPORT const unsigned long int
hypercore_crypto_version_patch();

/**
 * Returns the minor revision part.
 */
HYPERCORE_CRYPTO_EXPORT const unsigned long int
hypercore_crypto_version_revision();

#endif
