#ifndef UINT64BE_VERSION_H
#define UINT64BE_VERSION_H

#include "platform.h"

/**
 * Returns the version string for the library.
 */
UINT64BE_EXPORT const char *
uint64be_version_string();

/**
 * Returns the 32 bit version number that encodes the
 * major, minor, patch, and revision version parts.
 */
UINT64BE_EXPORT const unsigned long int
uint64be_version();

/**
 * Returns the major version part.
 */
UINT64BE_EXPORT const unsigned long int
uint64be_version_major();

/**
 * Returns the minor version part.
 */
UINT64BE_EXPORT const unsigned long int
uint64be_version_minor();

/**
 * Returns the minor patch part.
 */
UINT64BE_EXPORT const unsigned long int
uint64be_version_patch();

/**
 * Returns the minor revision part.
 */
UINT64BE_EXPORT const unsigned long int
uint64be_version_revision();

#endif
