#ifndef _HYPERCORE_CRYPTO_REQUIRE_H
#define _HYPERCORE_CRYPTO_REQUIRE_H

#include <errno.h>

#define require(op, code) { \
  if (0 == (op)) {          \
    errno = code;           \
    return -errno;          \
  }                         \
}

#endif
