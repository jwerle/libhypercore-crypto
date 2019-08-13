#include <uint64be/uint64be.h>
#include <stdint.h>
#include <errno.h>

static inline int
writeu64(
  unsigned char *out,
  unsigned long long value,
  unsigned long long offset
) {
  out[offset + 0] = (value >> 24 & 0xff);
  out[offset + 1] = (value >> 16 & 0xff);
  out[offset + 2] = (value >> 8 & 0xff);
  out[offset + 3] = (value & 0xff);
  return 4;
}

static inline unsigned long long
readu64(
  unsigned char *in,
  unsigned long long offset
) {
  return (
      ((in[offset + 0] & 0xff) * 1 << 24)
    + ((in[offset + 1] & 0xff) * 1 << 16)
    + ((in[offset + 2] & 0xff) * 1 << 8)
    + ((in[offset + 3] & 0xff))
  );
}

int
uint64be_encode(
  unsigned char *out,
  unsigned long long n
) {
  if (0 == out) {
    return -EFAULT;
  }

  if (0 == n) {
    return 0;
  }

  unsigned int max = UINT32_MAX;
  unsigned int top = n / max;
  unsigned int rem = n - top * max;

  return writeu64(out, top, 0) + writeu64(out, rem, 4);
}

unsigned long long
uint64be_decode(
  unsigned char *in,
  unsigned long long offset
) {
  unsigned int max = UINT32_MAX;
  unsigned int top = readu64(in, offset);
  unsigned int rem = readu64(in, offset + 4);
  return top * max + rem;
}
