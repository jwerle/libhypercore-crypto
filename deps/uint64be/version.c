#include "uint64be/version.h"

#define SX(x) #x
#define S(x) SX(x)

#ifndef UINT64BE_VERSION
#define UINT64BE_VERSION 0
#endif

#ifndef UINT64BE_NAME
#define UINT64BE_NAME "libuint64be"
#endif

#ifndef UINT64BE_VERSION_MAJOR
#define UINT64BE_VERSION_MAJOR 0
#endif

#ifndef UINT64BE_VERSION_MINOR
#define UINT64BE_VERSION_MINOR 0
#endif

#ifndef UINT64BE_VERSION_PATCH
#define UINT64BE_VERSION_PATCH 0
#endif

#ifndef UINT64BE_VERSION_REVISION
#define UINT64BE_VERSION_REVISION 0
#endif

#ifndef UINT64BE_DATE_COMPILED
#define UINT64BE_DATE_COMPILED ""
#endif

const char *
uint64be_version_string() {
  return UINT64BE_NAME
    "@" S(UINT64BE_VERSION_MAJOR)
    "." S(UINT64BE_VERSION_MINOR)
    "." S(UINT64BE_VERSION_PATCH)
    "." S(UINT64BE_VERSION_REVISION) " (" UINT64BE_DATE_COMPILED ")";
}

const unsigned long int
uint64be_version() {
  return (const unsigned long int) UINT64BE_VERSION;
}

const unsigned long int
uint64be_version_major() {
  return UINT64BE_VERSION >> 24 & 0xff;
}

const unsigned long int
uint64be_version_minor() {
  return UINT64BE_VERSION >> 16 & 0xff;
}

const unsigned long int
uint64be_version_patch() {
  return UINT64BE_VERSION >> 8 & 0xff;
}

const unsigned long int
uint64be_version_revision() {
  return UINT64BE_VERSION & 0xff;
}
