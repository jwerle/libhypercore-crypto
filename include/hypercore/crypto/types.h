#ifndef HYPERCORE_CRYPTO_TYPES_H
#define HYPERCORE_CRYPTO_TYPES_H

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

#endif
