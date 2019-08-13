#ifndef UINT64BE_H
#define UINT64BE_H

#include "platform.h"
#include "version.h"

UINT64BE_EXPORT int
uint64be_encode(
  unsigned char *out,
  unsigned long long n);

UINT64BE_EXPORT unsigned long long int
uint64be_decode(
  unsigned char *in,
  unsigned long long offset);

#endif
