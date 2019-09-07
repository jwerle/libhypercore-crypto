#ifndef HYPERCORE_CRYPTO_H
#define HYPERCORE_CRYPTO_H

#include "allocator.h"
#include "platform.h"
#include "version.h"

typedef struct hypercore_crypto_keypair hypercore_crypto_keypair_t;
typedef struct hypercore_crypto_buffer hypercore_crypto_buffer_t;
typedef struct hypercore_crypto_node hypercore_crypto_node_t;

#define hypercore_crypto_randombytes_BYTES 32
#define hypercore_crypto_data_BYTES 32
#define hypercore_crypto_key_BYTES 9

#define HYPERCORE_CRYPTO_LEAF_BYTE 0x00
#define HYPERCORE_CRYPTO_PARENT_BYTE 0x01
#define HYPERCORE_CRYPTO_ROOT_BYTE 0x02

#define HYPERCORE_CRYPTO_KEY_BYTES ((unsigned char []) { \
  0x68, 0x79, 0x70, 0x65,                                \
  0x72, 0x63, 0x6f, 0x72,                                \
  0x65                                                   \
})

struct hypercore_crypto_buffer {
  unsigned long long size;
  unsigned char *bytes;
};

struct hypercore_crypto_keypair {
  hypercore_crypto_buffer_t public_key;
  hypercore_crypto_buffer_t secret_key;
};

struct hypercore_crypto_node {
  unsigned long int index;
  unsigned long int size;
  hypercore_crypto_buffer_t *hash;
  hypercore_crypto_buffer_t *data;
};

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
