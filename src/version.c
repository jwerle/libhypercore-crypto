#include "hypercore/crypto/version.h"

#define SX(x) #x
#define S(x) SX(x)

#ifndef HYPERCORE_CRYPTO_VERSION
#define HYPERCORE_CRYPTO_VERSION 0
#endif

#ifndef HYPERCORE_CRYPTO_NAME
#define HYPERCORE_CRYPTO_NAME "libhypercore-crypto"
#endif

#ifndef HYPERCORE_CRYPTO_VERSION_MAJOR
#define HYPERCORE_CRYPTO_VERSION_MAJOR 0
#endif

#ifndef HYPERCORE_CRYPTO_VERSION_MINOR
#define HYPERCORE_CRYPTO_VERSION_MINOR 0
#endif

#ifndef HYPERCORE_CRYPTO_VERSION_PATCH
#define HYPERCORE_CRYPTO_VERSION_PATCH 0
#endif

#ifndef HYPERCORE_CRYPTO_VERSION_REVISION
#define HYPERCORE_CRYPTO_VERSION_REVISION 0
#endif

#ifndef HYPERCORE_CRYPTO_DATE_COMPILED
#define HYPERCORE_CRYPTO_DATE_COMPILED ""
#endif

const char *
hypercore_crypto_version_string() {
  return HYPERCORE_CRYPTO_NAME
    "@" S(HYPERCORE_CRYPTO_VERSION_MAJOR)
    "." S(HYPERCORE_CRYPTO_VERSION_MINOR)
    "." S(HYPERCORE_CRYPTO_VERSION_PATCH)
    "." S(HYPERCORE_CRYPTO_VERSION_REVISION) " (" HYPERCORE_CRYPTO_DATE_COMPILED ")";
}

const unsigned long int
hypercore_crypto_version() {
  return (const unsigned long int) HYPERCORE_CRYPTO_VERSION;
}

const unsigned long int
hypercore_crypto_version_major() {
  return HYPERCORE_CRYPTO_VERSION >> 24 & 0xff;
}

const unsigned long int
hypercore_crypto_version_minor() {
  return HYPERCORE_CRYPTO_VERSION >> 16 & 0xff;
}

const unsigned long int
hypercore_crypto_version_patch() {
  return HYPERCORE_CRYPTO_VERSION >> 8 & 0xff;
}

const unsigned long int
hypercore_crypto_version_revision() {
  return HYPERCORE_CRYPTO_VERSION & 0xff;
}
