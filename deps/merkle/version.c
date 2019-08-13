#include "merkle/version.h"

#define SX(x) #x
#define S(x) SX(x)

#ifndef MERKLE_VERSION
#define MERKLE_VERSION 0
#endif

#ifndef MERKLE_NAME
#define MERKLE_NAME "libmerkle"
#endif

#ifndef MERKLE_VERSION_MAJOR
#define MERKLE_VERSION_MAJOR 0
#endif

#ifndef MERKLE_VERSION_MINOR
#define MERKLE_VERSION_MINOR 0
#endif

#ifndef MERKLE_VERSION_PATCH
#define MERKLE_VERSION_PATCH 0
#endif

#ifndef MERKLE_VERSION_REVISION
#define MERKLE_VERSION_REVISION 0
#endif

#ifndef MERKLE_DATE_COMPILED
#define MERKLE_DATE_COMPILED ""
#endif

const char *
merkle_version_string() {
  return MERKLE_NAME
    "@" S(MERKLE_VERSION_MAJOR)
    "." S(MERKLE_VERSION_MINOR)
    "." S(MERKLE_VERSION_PATCH)
    "." S(MERKLE_VERSION_REVISION) " (" MERKLE_DATE_COMPILED ")";
}

const unsigned long int
merkle_version() {
  return (const unsigned long int) MERKLE_VERSION;
}

const unsigned long int
merkle_version_major() {
  return MERKLE_VERSION >> 24 & 0xff;
}

const unsigned long int
merkle_version_minor() {
  return MERKLE_VERSION >> 16 & 0xff;
}

const unsigned long int
merkle_version_patch() {
  return MERKLE_VERSION >> 8 & 0xff;
}

const unsigned long int
merkle_version_revision() {
  return MERKLE_VERSION & 0xff;
}
