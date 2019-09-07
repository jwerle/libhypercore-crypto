#ifndef HYPERCORE_CRYPTO_H
#define HYPERCORE_CRYPTO_H

#include "allocator.h"
#include "platform.h"
#include "version.h"
#include "types.h"

HYPERCORE_CRYPTO_EXPORT int
hypercore_crypto_keypair(
  hypercore_crypto_keypair_t *keypair,
  const unsigned char *seed);

HYPERCORE_CRYPTO_EXPORT void
hypercore_crypto_keypair_destroy(hypercore_crypto_keypair_t *keypair);

HYPERCORE_CRYPTO_EXPORT int
hypercore_crypto_sign(
  hypercore_crypto_buffer_t *out,
  const hypercore_crypto_buffer_t *message,
  const hypercore_crypto_buffer_t *secret_key);

HYPERCORE_CRYPTO_EXPORT int
hypercore_crypto_verify(
  hypercore_crypto_buffer_t *signature,
  const hypercore_crypto_buffer_t *message,
  const hypercore_crypto_buffer_t *public_key);

HYPERCORE_CRYPTO_EXPORT int
hypercore_crypto_data(
  hypercore_crypto_buffer_t *out,
  const hypercore_crypto_buffer_t *data);

HYPERCORE_CRYPTO_EXPORT int
hypercore_crypto_leaf(
  hypercore_crypto_buffer_t *out,
  const hypercore_crypto_node_t *leaf);

HYPERCORE_CRYPTO_EXPORT int
hypercore_crypto_parent(
  hypercore_crypto_buffer_t *out,
  const hypercore_crypto_node_t *left,
  const hypercore_crypto_node_t *right);

HYPERCORE_CRYPTO_EXPORT int
hypercore_crypto_tree(
  hypercore_crypto_buffer_t *out,
  const hypercore_crypto_node_t **roots,
  unsigned long long count);

HYPERCORE_CRYPTO_EXPORT int
hypercore_crypto_randombytes(hypercore_crypto_buffer_t *out);

HYPERCORE_CRYPTO_EXPORT int
hypercore_crypto_discoverykey(
  hypercore_crypto_buffer_t *out,
  hypercore_crypto_buffer_t *tree);

#endif
